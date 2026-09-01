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

#include "mimedetector.h"
#include <QFileInfo>
#include <QFile>
#include <QMimeDatabase>

namespace vectorpdf::conversion
{

FileKind MimeDetector::detectFileKind(const QString& filePath)
{
    QFileInfo info(filePath);
    const QString ext = info.suffix().trimmed().toLower();

    if (ext == QLatin1String("pdf"))
        return FileKind::Pdf;

    if (ext == QLatin1String("png") || ext == QLatin1String("jpg") || ext == QLatin1String("jpeg") ||
        ext == QLatin1String("tif") || ext == QLatin1String("tiff") || ext == QLatin1String("webp") ||
        ext == QLatin1String("bmp"))
        return FileKind::Image;

    if (ext == QLatin1String("md") || ext == QLatin1String("markdown"))
        return FileKind::Markdown;

    if (ext == QLatin1String("html") || ext == QLatin1String("htm"))
        return FileKind::Html;

    if (ext == QLatin1String("doc") || ext == QLatin1String("docx"))
        return FileKind::OfficeDocument;

    if (ext == QLatin1String("xfdf") || ext == QLatin1String("fdf"))
        return FileKind::FormData;

    // Check magic bytes if extension is unknown or missing
    if (QFile::exists(filePath))
    {
        QFile f(filePath);
        if (f.open(QIODevice::ReadOnly))
        {
            const QByteArray header = f.read(16);
            if (header.startsWith("%PDF-"))
                return FileKind::Pdf;
            if (header.startsWith("\x89PNG") || header.startsWith("\xFF\xD8\xFF") ||
                header.startsWith("BM") || header.startsWith("II*\0") || header.startsWith("MM\0*") ||
                (header.startsWith("RIFF") && header.mid(8, 4) == "WEBP"))
                return FileKind::Image;
            if (header.startsWith("PK\x03\x04"))
                return FileKind::OfficeDocument; // likely docx/xlsx/pptx/zip
        }
    }

    return FileKind::Unknown;
}

QString MimeDetector::detectMimeType(const QString& filePath)
{
    QMimeDatabase db;
    return db.mimeTypeForFile(filePath).name();
}

bool MimeDetector::isSupportedSource(const QString& filePath)
{
    return detectFileKind(filePath) != FileKind::Unknown;
}

} // namespace vectorpdf::conversion
