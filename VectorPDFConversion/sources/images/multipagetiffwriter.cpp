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

#include "multipagetiffwriter.h"
#include <QFile>
#include <QDataStream>
#include <QFileInfo>
#include <zlib.h>

namespace vectorpdf::conversion
{

namespace
{
struct TiffTag
{
    quint16 tag;
    quint16 type; // 3 = SHORT (2 bytes), 4 = LONG (4 bytes), 5 = RATIONAL (8 bytes)
    quint32 count;
    quint32 valueOrOffset;
};

void writeTag(QDataStream& out, quint16 tag, quint16 type, quint32 count, quint32 val)
{
    out << tag << type << count << val;
}
} // namespace

bool MultiPageTiffWriter::writeMultiPageTiff(const QList<QImage>& pages,
                                            const QString& targetTiffPath,
                                            int dpi,
                                            QString* errorMessage)
{
    if (pages.isEmpty())
    {
        if (errorMessage) *errorMessage = QStringLiteral("No pages provided for multi-page TIFF.");
        return false;
    }

    QFile file(targetTiffPath);
    if (!file.open(QIODevice::WriteOnly))
    {
        if (errorMessage) *errorMessage = QStringLiteral("Cannot open destination TIFF file: '%1'").arg(targetTiffPath);
        return false;
    }

    QDataStream out(&file);
    out.setByteOrder(QDataStream::LittleEndian);

    // 1. TIFF Header: 'II' (0x4949) + 42 (0x002A) + offset to First IFD (8)
    out << static_cast<quint16>(0x4949); // Little endian
    out << static_cast<quint16>(42);
    out << static_cast<quint32>(8);      // First IFD offset is immediately after header

    const int pageCount = pages.size();
    const quint32 targetDpi = dpi > 0 ? dpi : 300;

    // We will build and write each page sequentially:
    // [IFD_i] (12 entries * 12 bytes + 2 count bytes + 4 next_ifd bytes = 150 bytes)
    // [Aux Data for IFD_i: BitsPerSample (6 bytes), XRes (8 bytes), YRes (8 bytes) = 22 bytes]
    // [Pixel Strip Data for Page i]
    // -> Next IFD offset

    for (int p = 0; p < pageCount; ++p)
    {
        QImage img = pages[p].convertToFormat(QImage::Format_RGB888);
        const quint32 width = static_cast<quint32>(img.width());
        const quint32 height = static_cast<quint32>(img.height());
        const quint32 bytesPerLine = width * 3;
        const quint32 pixelDataSize = bytesPerLine * height;

        const qint64 currentIfdOffset = file.pos();
        const quint16 tagCount = 13;
        const quint32 ifdSize = 2 + (tagCount * 12) + 4;

        const quint32 bitsPerSampleOffset = currentIfdOffset + ifdSize;
        const quint32 xResOffset = bitsPerSampleOffset + 6;
        const quint32 yResOffset = xResOffset + 8;
        const quint32 pixelDataOffset = yResOffset + 8;
        const quint32 nextIfdOffset = (p < pageCount - 1) ? (pixelDataOffset + pixelDataSize) : 0;

        // Write Tag Count
        out << tagCount;

        // Tags must be written in ascending numerical order:
        // 254: NewSubfileType (0 for primary image / multi-page)
        writeTag(out, 254, 4, 1, 0);
        // 256: ImageWidth
        writeTag(out, 256, 4, 1, width);
        // 257: ImageLength
        writeTag(out, 257, 4, 1, height);
        // 258: BitsPerSample (3 values of SHORT: 8, 8, 8 -> pointer)
        writeTag(out, 258, 3, 3, bitsPerSampleOffset);
        // 259: Compression (1 = uncompressed)
        writeTag(out, 259, 3, 1, 1);
        // 262: PhotometricInterpretation (2 = RGB)
        writeTag(out, 262, 3, 1, 2);
        // 273: StripOffsets
        writeTag(out, 273, 4, 1, pixelDataOffset);
        // 277: SamplesPerPixel (3 = RGB)
        writeTag(out, 277, 3, 1, 3);
        // 278: RowsPerStrip (height)
        writeTag(out, 278, 4, 1, height);
        // 279: StripByteCounts
        writeTag(out, 279, 4, 1, pixelDataSize);
        // 282: XResolution (RATIONAL -> pointer)
        writeTag(out, 282, 5, 1, xResOffset);
        // 283: YResolution (RATIONAL -> pointer)
        writeTag(out, 283, 5, 1, yResOffset);
        // 296: ResolutionUnit (2 = Inch)
        writeTag(out, 296, 3, 1, 2);

        // Next IFD Offset
        out << nextIfdOffset;

        // Write Aux Data:
        // BitsPerSample: 8, 8, 8 (3 x quint16 = 6 bytes)
        out << static_cast<quint16>(8) << static_cast<quint16>(8) << static_cast<quint16>(8);
        // XResolution: targetDpi / 1
        out << targetDpi << static_cast<quint32>(1);
        // YResolution: targetDpi / 1
        out << targetDpi << static_cast<quint32>(1);

        // Write Pixel Data
        for (int y = 0; y < img.height(); ++y)
        {
            const uchar* scanLine = img.constScanLine(y);
            file.write(reinterpret_cast<const char*>(scanLine), bytesPerLine);
        }
    }

    file.close();
    return true;
}

int MultiPageTiffWriter::countDirectories(const QString& tiffPath, QString* errorMessage)
{
    QFile file(tiffPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        if (errorMessage) *errorMessage = QStringLiteral("Cannot open TIFF for inspection: '%1'").arg(tiffPath);
        return -1;
    }

    QDataStream in(&file);
    quint16 byteOrder = 0;
    in >> byteOrder;

    if (byteOrder == 0x4949)
    {
        in.setByteOrder(QDataStream::LittleEndian);
    }
    else if (byteOrder == 0x4D4D)
    {
        in.setByteOrder(QDataStream::BigEndian);
    }
    else
    {
        if (errorMessage) *errorMessage = QStringLiteral("Invalid TIFF byte order magic.");
        return -1;
    }

    quint16 magic = 0;
    in >> magic;
    if (magic != 42)
    {
        if (errorMessage) *errorMessage = QStringLiteral("Invalid TIFF magic number 42.");
        return -1;
    }

    quint32 nextIfdOffset = 0;
    in >> nextIfdOffset;

    int dirCount = 0;
    while (nextIfdOffset != 0 && nextIfdOffset < file.size())
    {
        if (!file.seek(nextIfdOffset)) break;

        quint16 numEntries = 0;
        in >> numEntries;
        dirCount++;

        // Skip entries (each 12 bytes)
        file.seek(file.pos() + (numEntries * 12));
        in >> nextIfdOffset;
    }

    return dirCount;
}

} // namespace vectorpdf::conversion
