// MIT License
//
// Copyright (c) 2026 VectorPDF Contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "ooxmlpackagevalidator.h"
#include <QFile>
#include <QDataStream>
#include <QXmlStreamReader>
#include <zlib.h>

namespace vectorpdf::conversion
{

namespace
{
struct ZipLocalEntry
{
    QString fileName;
    quint16 method = 0;
    quint32 compressedSize = 0;
    quint32 uncompressedSize = 0;
    quint32 dataOffset = 0;
};

QByteArray decompressZipEntry(QFile& file, const ZipLocalEntry& entry)
{
    if (!file.seek(entry.dataOffset)) return QByteArray();

    QByteArray compData = file.read(entry.compressedSize);
    if (compData.size() != static_cast<int>(entry.compressedSize)) return QByteArray();

    if (entry.method == 0) // Stored / uncompressed
    {
        return compData;
    }
    else if (entry.method == 8) // Deflate
    {
        QByteArray uncompData(entry.uncompressedSize, 0);

        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = compData.size();
        strm.next_in = reinterpret_cast<Bytef*>(compData.data());
        strm.avail_out = uncompData.size();
        strm.next_out = reinterpret_cast<Bytef*>(uncompData.data());

        // -MAX_WBITS for raw deflate stream in ZIP
        if (inflateInit2(&strm, -MAX_WBITS) != Z_OK) return QByteArray();

        int ret = inflate(&strm, Z_FINISH);
        inflateEnd(&strm);

        if (ret == Z_STREAM_END || ret == Z_OK)
        {
            uncompData.resize(strm.total_out);
            return uncompData;
        }
    }
    return QByteArray();
}
} // namespace

QStringList OoxmlPackageValidator::listZipEntries(const QString& zipFilePath, QString* errorMessage)
{
    QStringList entries;
    QFile file(zipFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        if (errorMessage) *errorMessage = QStringLiteral("Cannot open ZIP package file: '%1'").arg(zipFilePath);
        return entries;
    }

    const qint64 fileSize = file.size();
    if (fileSize < 22)
    {
        if (errorMessage) *errorMessage = QStringLiteral("File size too small for a valid ZIP archive.");
        return entries;
    }

    // Find EOCD (0x06054b50) scanning backwards up to 65KB
    const qint64 maxScan = qMin<qint64>(fileSize, 65536 + 22);
    file.seek(fileSize - maxScan);
    QByteArray buf = file.read(maxScan);

    qint64 eocdPos = -1;
    for (qint64 i = buf.size() - 22; i >= 0; --i)
    {
        if (static_cast<uchar>(buf[i]) == 0x50 &&
            static_cast<uchar>(buf[i+1]) == 0x4B &&
            static_cast<uchar>(buf[i+2]) == 0x05 &&
            static_cast<uchar>(buf[i+3]) == 0x06)
        {
            eocdPos = (fileSize - maxScan) + i;
            break;
        }
    }

    if (eocdPos < 0)
    {
        if (errorMessage) *errorMessage = QStringLiteral("End of Central Directory record not found.");
        return entries;
    }

    file.seek(eocdPos);
    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);

    quint32 sig = 0;
    in >> sig; // 0x06054b50
    quint16 diskNum = 0, startDisk = 0, numEntriesOnDisk = 0, totalEntries = 0;
    quint32 cdSize = 0, cdOffset = 0;
    in >> diskNum >> startDisk >> numEntriesOnDisk >> totalEntries >> cdSize >> cdOffset;

    file.seek(cdOffset);
    for (int i = 0; i < totalEntries; ++i)
    {
        quint32 cdSig = 0;
        in >> cdSig;
        if (cdSig != 0x02014B50) break;

        quint16 vMade = 0, vNeeded = 0, flags = 0, method = 0, modTime = 0, modDate = 0;
        quint32 crc = 0, compSize = 0, uncompSize = 0;
        quint16 fnameLen = 0, extraLen = 0, commentLen = 0, diskStart = 0, intAttr = 0;
        quint32 extAttr = 0, localOffset = 0;

        in >> vMade >> vNeeded >> flags >> method >> modTime >> modDate >> crc >> compSize >> uncompSize
           >> fnameLen >> extraLen >> commentLen >> diskStart >> intAttr >> extAttr >> localOffset;

        QByteArray fnameBytes = file.read(fnameLen);
        QString fname = QString::fromUtf8(fnameBytes);
        entries.append(fname);

        file.seek(file.pos() + extraLen + commentLen);
    }

    return entries;
}

OoxmlValidationResult OoxmlPackageValidator::validatePackage(const QString& filePath, ConversionFormat format)
{
    OoxmlValidationResult result;
    result.isValid = false;

    QString listErr;
    result.archiveEntries = listZipEntries(filePath, &listErr);
    if (result.archiveEntries.isEmpty())
    {
        result.errorMessage = listErr.isEmpty() ? QStringLiteral("Empty ZIP archive or corrupt central directory.") : listErr;
        return result;
    }

    // 1. Universal OPC required parts
    QStringList requiredParts = {
        QStringLiteral("[Content_Types].xml"),
        QStringLiteral("_rels/.rels")
    };

    // 2. Format specific required parts
    if (format == ConversionFormat::Docx)
    {
        requiredParts.append(QStringLiteral("word/document.xml"));
    }
    else if (format == ConversionFormat::Xlsx)
    {
        requiredParts.append(QStringLiteral("xl/workbook.xml"));
    }
    else if (format == ConversionFormat::Pptx)
    {
        requiredParts.append(QStringLiteral("ppt/presentation.xml"));
    }

    for (const QString& req : requiredParts)
    {
        if (!result.archiveEntries.contains(req))
        {
            result.missingRequiredParts.append(req);
        }
    }

    // Check at least one sheet for XLSX
    if (format == ConversionFormat::Xlsx)
    {
        bool hasSheet = false;
        for (const QString& entry : result.archiveEntries)
        {
            if (entry.startsWith(QStringLiteral("xl/worksheets/sheet")) && entry.endsWith(QStringLiteral(".xml")))
            {
                hasSheet = true;
                break;
            }
        }
        if (!hasSheet)
        {
            result.missingRequiredParts.append(QStringLiteral("xl/worksheets/sheet1.xml"));
        }
    }

    // Check at least one slide for PPTX
    if (format == ConversionFormat::Pptx)
    {
        bool hasSlide = false;
        for (const QString& entry : result.archiveEntries)
        {
            if (entry.startsWith(QStringLiteral("ppt/slides/slide")) && entry.endsWith(QStringLiteral(".xml")))
            {
                hasSlide = true;
                break;
            }
        }
        if (!hasSlide)
        {
            result.missingRequiredParts.append(QStringLiteral("ppt/slides/slide1.xml"));
        }
    }

    if (!result.missingRequiredParts.isEmpty())
    {
        result.errorMessage = QStringLiteral("Missing required OpenXML parts: %1").arg(result.missingRequiredParts.join(QStringLiteral(", ")));
        return result;
    }

    // 3. Decompress and validate XML syntax for core parts
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        // Enumerate local files and parse XML
        QDataStream in(&file);
        in.setByteOrder(QDataStream::LittleEndian);

        for (const QString& reqPart : requiredParts)
        {
            file.seek(0);
            while (!file.atEnd())
            {
                quint32 localSig = 0;
                in >> localSig;
                if (localSig != 0x04034B50) break;

                quint16 vNeed = 0, flags = 0, method = 0, modTime = 0, modDate = 0;
                quint32 crc = 0, compSize = 0, uncompSize = 0;
                quint16 fnameLen = 0, extraLen = 0;

                in >> vNeed >> flags >> method >> modTime >> modDate >> crc >> compSize >> uncompSize >> fnameLen >> extraLen;

                QByteArray fnameBytes = file.read(fnameLen);
                QString fname = QString::fromUtf8(fnameBytes);
                file.seek(file.pos() + extraLen);

                ZipLocalEntry entry;
                entry.fileName = fname;
                entry.method = method;
                entry.compressedSize = compSize;
                entry.uncompressedSize = uncompSize;
                entry.dataOffset = file.pos();

                if (fname == reqPart)
                {
                    QByteArray xmlData = decompressZipEntry(file, entry);
                    if (xmlData.isEmpty())
                    {
                        result.xmlValidationErrors.append(QStringLiteral("Failed to decompress part '%1'").arg(reqPart));
                    }
                    else
                    {
                        QXmlStreamReader xml(xmlData);
                        while (!xml.atEnd())
                        {
                            xml.readNext();
                        }
                        if (xml.hasError())
                        {
                            result.xmlValidationErrors.append(QStringLiteral("XML error in '%1': %2").arg(reqPart).arg(xml.errorString()));
                        }
                    }
                    break;
                }

                file.seek(entry.dataOffset + compSize);
            }
        }
    }

    if (!result.xmlValidationErrors.isEmpty())
    {
        result.errorMessage = QStringLiteral("XML syntax validation failed: %1").arg(result.xmlValidationErrors.join(QStringLiteral("; ")));
        return result;
    }

    result.isValid = true;
    return result;
}

} // namespace vectorpdf::conversion
