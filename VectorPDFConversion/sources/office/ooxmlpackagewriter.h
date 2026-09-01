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

#ifndef VECTORPDF_OOXMLPACKAGEWRITER_H
#define VECTORPDF_OOXMLPACKAGEWRITER_H

#include "../conversionglobal.h"
#include <QString>
#include <QByteArray>
#include <QMap>
#include <QList>
#include <QDateTime>

namespace vectorpdf::conversion
{

struct ZipEntry
{
    QString path;
    QByteArray data;
    bool compress = true;
};

class VECTORPDF_CONVERSION_EXPORT OoxmlPackageWriter
{
public:
    OoxmlPackageWriter();
    ~OoxmlPackageWriter() = default;

    /// Adds a file entry with raw byte contents to the package
    void addEntry(const QString& entryPath, const QByteArray& data, bool compress = true);

    /// Adds an XML or text entry
    void addTextEntry(const QString& entryPath, const QString& text, bool compress = true);

    /// Generates and writes standard [Content_Types].xml based on added entries
    void addDefaultContentTypes();

    /// Writes the full package into a target .zip / .docx / .xlsx / .pptx file path
    bool save(const QString& targetFilePath, QString* errorMessage = nullptr);

    /// Clears all entries
    void clear();

private:
    QMap<QString, ZipEntry> m_entries;
    QMap<QString, QString> m_extensionContentTypes;
    QMap<QString, QString> m_overrideContentTypes;
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_OOXMLPACKAGEWRITER_H
