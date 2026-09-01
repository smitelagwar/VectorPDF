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

#include "ooxmlpackagewriter.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDataStream>
#include <QXmlStreamWriter>
#include <zlib.h>

namespace vectorpdf::conversion
{

namespace
{

quint32 calculateCrc32(const QByteArray& data)
{
    return static_cast<quint32>(crc32(0, reinterpret_cast<const Bytef*>(data.constData()), static_cast<uInt>(data.size())));
}

QByteArray rawDeflate(const QByteArray& input)
{
    if (input.isEmpty()) return QByteArray();

    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    // Negative MAX_WBITS (-15) for raw deflate stream without zlib/gzip headers
    if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK)
    {
        return QByteArray();
    }

    strm.avail_in = static_cast<uInt>(input.size());
    strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.constData()));

    QByteArray output;
    output.resize(input.size() + 1024);

    strm.avail_out = static_cast<uInt>(output.size());
    strm.next_out = reinterpret_cast<Bytef*>(output.data());

    int ret = deflate(&strm, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK)
    {
        deflateEnd(&strm);
        return QByteArray();
    }

    output.resize(static_cast<int>(strm.total_out));
    deflateEnd(&strm);
    return output;
}

quint16 dosTime(const QDateTime& dt)
{
    const QTime t = dt.time();
    return static_cast<quint16>((t.hour() << 11) | (t.minute() << 5) | (t.second() / 2));
}

quint16 dosDate(const QDateTime& dt)
{
    const QDate d = dt.date();
    int year = d.year();
    if (year < 1980) year = 1980;
    return static_cast<quint16>(((year - 1980) << 9) | (d.month() << 5) | d.day());
}

} // namespace

OoxmlPackageWriter::OoxmlPackageWriter()
{
    m_extensionContentTypes[QStringLiteral("rels")] = QStringLiteral("application/vnd.openxmlformats-package.relationships+xml");
    m_extensionContentTypes[QStringLiteral("xml")] = QStringLiteral("application/xml");
    m_extensionContentTypes[QStringLiteral("png")] = QStringLiteral("image/png");
    m_extensionContentTypes[QStringLiteral("jpeg")] = QStringLiteral("image/jpeg");
    m_extensionContentTypes[QStringLiteral("jpg")] = QStringLiteral("image/jpeg");
    m_extensionContentTypes[QStringLiteral("webp")] = QStringLiteral("image/webp");
}

void OoxmlPackageWriter::addEntry(const QString& entryPath, const QByteArray& data, bool compress)
{
    QString normalized = entryPath;
    normalized.replace(QLatin1Char('\\'), QLatin1Char('/'));
    if (normalized.startsWith(QLatin1Char('/')))
    {
        normalized = normalized.mid(1);
    }

    ZipEntry entry;
    entry.path = normalized;
    entry.data = data;
    entry.compress = compress;

    m_entries[normalized] = entry;
}

void OoxmlPackageWriter::addTextEntry(const QString& entryPath, const QString& text, bool compress)
{
    addEntry(entryPath, text.toUtf8(), compress);
}

void OoxmlPackageWriter::addDefaultContentTypes()
{
    QString xmlString;
    QXmlStreamWriter xml(&xmlString);
    xml.setAutoFormatting(true);
    xml.writeStartDocument(QStringLiteral("1.0"), true);
    xml.writeStartElement(QStringLiteral("Types"));
    xml.writeAttribute(QStringLiteral("xmlns"), QStringLiteral("http://schemas.openxmlformats.org/package/2006/content-types"));

    // Write defaults
    for (auto it = m_extensionContentTypes.constBegin(); it != m_extensionContentTypes.constEnd(); ++it)
    {
        xml.writeStartElement(QStringLiteral("Default"));
        xml.writeAttribute(QStringLiteral("Extension"), it.key());
        xml.writeAttribute(QStringLiteral("ContentType"), it.value());
        xml.writeEndElement();
    }

    // Write overrides
    for (auto it = m_overrideContentTypes.constBegin(); it != m_overrideContentTypes.constEnd(); ++it)
    {
        xml.writeStartElement(QStringLiteral("Override"));
        xml.writeAttribute(QStringLiteral("PartName"), it.key().startsWith(QLatin1Char('/')) ? it.key() : QLatin1Char('/') + it.key());
        xml.writeAttribute(QStringLiteral("ContentType"), it.value());
        xml.writeEndElement();
    }

    xml.writeEndElement();
    xml.writeEndDocument();

    addEntry(QStringLiteral("[Content_Types].xml"), xmlString.toUtf8(), true);
}

bool OoxmlPackageWriter::save(const QString& targetFilePath, QString* errorMessage)
{
    QFile file(targetFilePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        if (errorMessage) *errorMessage = QStringLiteral("Cannot open target file '%1' for writing: %2").arg(targetFilePath).arg(file.errorString());
        return false;
    }

    struct CentralDirRecord
    {
        QString path;
        quint32 crc = 0;
        quint32 compressedSize = 0;
        quint32 uncompressedSize = 0;
        quint32 localHeaderOffset = 0;
        quint16 compressionMethod = 0;
        quint16 time = 0;
        quint16 date = 0;
    };

    QList<CentralDirRecord> centralDir;
    const QDateTime now = QDateTime::currentDateTime();
    const quint16 timeDos = dosTime(now);
    const quint16 dateDos = dosDate(now);

    QDataStream out(&file);
    out.setByteOrder(QDataStream::LittleEndian);

    for (const ZipEntry& entry : m_entries)
    {
        const QByteArray pathUtf8 = entry.path.toUtf8();
        const quint32 crc = calculateCrc32(entry.data);
        const quint32 uncompressedSize = static_cast<quint32>(entry.data.size());

        QByteArray payload;
        quint16 method = 0;

        if (entry.compress && uncompressedSize > 0)
        {
            QByteArray deflated = rawDeflate(entry.data);
            if (!deflated.isEmpty() && deflated.size() < entry.data.size())
            {
                payload = deflated;
                method = 8; // Deflate
            }
            else
            {
                payload = entry.data;
                method = 0; // Store
            }
        }
        else
        {
            payload = entry.data;
            method = 0; // Store
        }

        const quint32 compressedSize = static_cast<quint32>(payload.size());
        const quint32 localOffset = static_cast<quint32>(file.pos());

        // Write Local File Header
        out << static_cast<quint32>(0x04034b50); // Signature
        out << static_cast<quint16>(20);         // Version needed (2.0)
        out << static_cast<quint16>(0x0800);     // Flags (UTF-8 filename)
        out << method;                           // Compression method
        out << timeDos;                          // Last mod time
        out << dateDos;                          // Last mod date
        out << crc;                              // CRC32
        out << compressedSize;                   // Compressed size
        out << uncompressedSize;                 // Uncompressed size
        out << static_cast<quint16>(pathUtf8.size()); // File name length
        out << static_cast<quint16>(0);          // Extra field length

        file.write(pathUtf8);
        file.write(payload);

        CentralDirRecord cdr;
        cdr.path = entry.path;
        cdr.crc = crc;
        cdr.compressedSize = compressedSize;
        cdr.uncompressedSize = uncompressedSize;
        cdr.localHeaderOffset = localOffset;
        cdr.compressionMethod = method;
        cdr.time = timeDos;
        cdr.date = dateDos;
        centralDir.append(cdr);
    }

    const quint32 centralDirStart = static_cast<quint32>(file.pos());

    // Write Central Directory Headers
    for (const CentralDirRecord& cdr : centralDir)
    {
        const QByteArray pathUtf8 = cdr.path.toUtf8();

        out << static_cast<quint32>(0x02014b50); // Central directory signature
        out << static_cast<quint16>(20);         // Version made by
        out << static_cast<quint16>(20);         // Version needed
        out << static_cast<quint16>(0x0800);     // Flags (UTF-8)
        out << cdr.compressionMethod;
        out << cdr.time;
        out << cdr.date;
        out << cdr.crc;
        out << cdr.compressedSize;
        out << cdr.uncompressedSize;
        out << static_cast<quint16>(pathUtf8.size()); // Filename length
        out << static_cast<quint16>(0);          // Extra field length
        out << static_cast<quint16>(0);          // Comment length
        out << static_cast<quint16>(0);          // Disk number start
        out << static_cast<quint16>(0);          // Internal attributes
        out << static_cast<quint32>(0);          // External attributes
        out << cdr.localHeaderOffset;            // Relative offset of local header

        file.write(pathUtf8);
    }

    const quint32 centralDirEnd = static_cast<quint32>(file.pos());
    const quint32 centralDirSize = centralDirEnd - centralDirStart;

    // Write End of Central Directory Record (EOCD)
    out << static_cast<quint32>(0x06054b50); // EOCD Signature
    out << static_cast<quint16>(0);          // Disk number
    out << static_cast<quint16>(0);          // Disk with central dir
    out << static_cast<quint16>(centralDir.size()); // Total entries this disk
    out << static_cast<quint16>(centralDir.size()); // Total entries
    out << centralDirSize;                   // Size of central dir
    out << centralDirStart;                  // Offset of central dir
    out << static_cast<quint16>(0);          // Comment length

    file.flush();
    file.close();

    return true;
}

void OoxmlPackageWriter::clear()
{
    m_entries.clear();
}

} // namespace vectorpdf::conversion
