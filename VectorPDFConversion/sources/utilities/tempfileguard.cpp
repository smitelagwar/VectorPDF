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

#include "tempfileguard.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QUuid>

namespace vectorpdf::conversion
{

TempFileGuard::TempFileGuard(const QString& prefix)
    : m_tempDir(std::make_unique<QTemporaryDir>(QDir::tempPath() + QLatin1Char('/') + prefix))
{
}

TempFileGuard::~TempFileGuard()
{
    if (!m_released)
    {
        for (const QString& file : m_trackedFiles)
        {
            if (QFile::exists(file))
            {
                QFile::remove(file);
            }
        }
    }
}

TempFileGuard::TempFileGuard(TempFileGuard&& other) noexcept
    : m_tempDir(std::move(other.m_tempDir))
    , m_trackedFiles(std::move(other.m_trackedFiles))
    , m_released(other.m_released)
{
    other.m_released = true;
}

TempFileGuard& TempFileGuard::operator=(TempFileGuard&& other) noexcept
{
    if (this != &other)
    {
        m_tempDir = std::move(other.m_tempDir);
        m_trackedFiles = std::move(other.m_trackedFiles);
        m_released = other.m_released;
        other.m_released = true;
    }
    return *this;
}

bool TempFileGuard::isValid() const
{
    return m_tempDir && m_tempDir->isValid();
}

QString TempFileGuard::dirPath() const
{
    return m_tempDir ? m_tempDir->path() : QString();
}

QString TempFileGuard::createTempFilePath(const QString& extension, const QString& prefix)
{
    if (!isValid()) return QString();

    const QString cleanExt = extension.startsWith(QLatin1Char('.')) ? extension.mid(1) : extension;
    const QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    const QString fileName = QStringLiteral("%1%2.%3").arg(prefix).arg(uuid).arg(cleanExt);

    return QDir(dirPath()).filePath(fileName);
}

void TempFileGuard::trackFile(const QString& filePath)
{
    if (!filePath.isEmpty() && !m_trackedFiles.contains(filePath))
    {
        m_trackedFiles.append(filePath);
    }
}

bool TempFileGuard::atomicCommit(const QString& tempFilePath, const QString& finalDestinationPath, QString* errorMessage)
{
    if (!QFile::exists(tempFilePath))
    {
        if (errorMessage) *errorMessage = QStringLiteral("Temporary file '%1' does not exist.").arg(tempFilePath);
        return false;
    }

    QFileInfo destInfo(finalDestinationPath);
    QDir destDir = destInfo.dir();
    if (!destDir.exists())
    {
        if (!destDir.mkpath(QStringLiteral(".")))
        {
            if (errorMessage) *errorMessage = QStringLiteral("Failed to create destination directory '%1'.").arg(destDir.path());
            return false;
        }
    }

    if (QFile::exists(finalDestinationPath))
    {
        // Safe replace: remove existing first
        if (!QFile::remove(finalDestinationPath))
        {
            if (errorMessage) *errorMessage = QStringLiteral("Failed to overwrite existing destination file '%1'.").arg(finalDestinationPath);
            return false;
        }
    }

    // Try fast atomic rename
    if (QFile::rename(tempFilePath, finalDestinationPath))
    {
        return true;
    }

    // Fallback: copy and remove
    if (QFile::copy(tempFilePath, finalDestinationPath))
    {
        QFile::remove(tempFilePath);
        return true;
    }

    if (errorMessage) *errorMessage = QStringLiteral("Failed to move temporary file '%1' to '%2'.").arg(tempFilePath).arg(finalDestinationPath);
    return false;
}

void TempFileGuard::release()
{
    m_released = true;
    if (m_tempDir)
    {
        m_tempDir->setAutoRemove(false);
    }
}

} // namespace vectorpdf::conversion
