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

#include "filenamepolicy.h"
#include <QFileInfo>
#include <QDir>
#include <QDateTime>

namespace vectorpdf::conversion
{

QString FilenamePolicy::computeDefaultOutputPath(const QString& sourcePath, ConversionFormat format, const QString& targetDirectory)
{
    QFileInfo srcInfo(sourcePath);
    QString baseName = srcInfo.completeBaseName();
    if (baseName.isEmpty())
    {
        baseName = QStringLiteral("document");
    }

    QString dir = targetDirectory;
    if (dir.isEmpty())
    {
        dir = srcInfo.exists() ? srcInfo.dir().path() : QDir::currentPath();
    }

    QString ext = formatDefaultExtension(format);
    QString suffix;

    switch (format)
    {
    case ConversionFormat::PdfA1:         suffix = QStringLiteral("_PDFA1"); break;
    case ConversionFormat::PdfA2:         suffix = QStringLiteral("_PDFA2"); break;
    case ConversionFormat::PdfA3:         suffix = QStringLiteral("_PDFA3"); break;
    case ConversionFormat::PdfA4:         suffix = QStringLiteral("_PDFA4"); break;
    case ConversionFormat::MonochromePdf: suffix = QStringLiteral("_monochrome"); break;
    default:                              suffix.clear(); break;
    }

    const QString fileName = QStringLiteral("%1%2.%3").arg(baseName).arg(suffix).arg(ext);
    return QDir(dir).filePath(fileName);
}

QString FilenamePolicy::formatPageFileName(const QString& templatePattern, const QString& documentBaseName, int pageIndex0Based, const QString& extension)
{
    const int pageNum = pageIndex0Based + 1;
    QString pattern = templatePattern;
    if (pattern.isEmpty())
    {
        pattern = QStringLiteral("{document}_page_{page:000}.{ext}");
    }

    const QString cleanExt = extension.startsWith(QLatin1Char('.')) ? extension.mid(1) : extension;

    pattern.replace(QStringLiteral("{document}"), documentBaseName);
    pattern.replace(QStringLiteral("{ext}"), cleanExt);
    pattern.replace(QStringLiteral("{date}"), QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd")));

    // Handle {page:000}, {page:00}, {page:0}, {page}
    if (pattern.contains(QStringLiteral("{page:000}")))
    {
        pattern.replace(QStringLiteral("{page:000}"), QStringLiteral("%1").arg(pageNum, 3, 10, QLatin1Char('0')));
    }
    if (pattern.contains(QStringLiteral("{page:00}")))
    {
        pattern.replace(QStringLiteral("{page:00}"), QStringLiteral("%1").arg(pageNum, 2, 10, QLatin1Char('0')));
    }
    if (pattern.contains(QStringLiteral("{page:0}")))
    {
        pattern.replace(QStringLiteral("{page:0}"), QStringLiteral("%1").arg(pageNum, 2, 10, QLatin1Char('0')));
    }
    pattern.replace(QStringLiteral("{page}"), QString::number(pageNum));

    if (!pattern.endsWith(QStringLiteral(".") + cleanExt, Qt::CaseInsensitive))
    {
        pattern.append(QStringLiteral(".") + cleanExt);
    }

    return pattern;
}

QString FilenamePolicy::resolveCollision(const QString& desiredFilePath, bool overwrite)
{
    if (overwrite || !QFile::exists(desiredFilePath))
    {
        return desiredFilePath;
    }

    QFileInfo info(desiredFilePath);
    const QDir dir = info.dir();
    const QString baseName = info.completeBaseName();
    const QString suffix = info.suffix();

    int counter = 1;
    while (counter < 10000)
    {
        const QString newName = suffix.isEmpty()
            ? QStringLiteral("%1 (%2)").arg(baseName).arg(counter)
            : QStringLiteral("%1 (%2).%3").arg(baseName).arg(counter).arg(suffix);

        const QString candidate = dir.filePath(newName);
        if (!QFile::exists(candidate))
        {
            return candidate;
        }
        ++counter;
    }

    return desiredFilePath;
}

bool FilenamePolicy::isSourceOverwrite(const QString& sourcePath, const QString& targetPath)
{
    if (sourcePath.isEmpty() || targetPath.isEmpty()) return false;
    const QString normSrc = QFileInfo(sourcePath).canonicalFilePath();
    const QString normTarget = QFileInfo(targetPath).canonicalFilePath();

    if (!normSrc.isEmpty() && !normTarget.isEmpty())
    {
        return QString::compare(normSrc, normTarget, Qt::CaseInsensitive) == 0;
    }

    return QString::compare(QDir::cleanPath(sourcePath), QDir::cleanPath(targetPath), Qt::CaseInsensitive) == 0;
}

} // namespace vectorpdf::conversion
