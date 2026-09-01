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

#ifndef VECTORPDF_TEMPFILEGUARD_H
#define VECTORPDF_TEMPFILEGUARD_H

#include "../conversionglobal.h"
#include <QString>
#include <QStringList>
#include <QTemporaryDir>
#include <memory>

namespace vectorpdf::conversion
{

class VECTORPDF_CONVERSION_EXPORT TempFileGuard
{
public:
    explicit TempFileGuard(const QString& prefix = QStringLiteral("vpdf_conv_"));
    ~TempFileGuard();

    TempFileGuard(const TempFileGuard&) = delete;
    TempFileGuard& operator=(const TempFileGuard&) = delete;

    TempFileGuard(TempFileGuard&& other) noexcept;
    TempFileGuard& operator=(TempFileGuard&& other) noexcept;

    bool isValid() const;
    QString dirPath() const;

    /// Creates a unique temporary file path inside this guard's sandbox directory
    QString createTempFilePath(const QString& extension, const QString& prefix = QStringLiteral("out_"));

    /// Tracks an external file path to be cleaned up when this guard is destroyed
    void trackFile(const QString& filePath);

    /// Atomically moves a temporary file to the final destination path
    static bool atomicCommit(const QString& tempFilePath, const QString& finalDestinationPath, QString* errorMessage = nullptr);

    /// Releases tracking so files won't be deleted on destruction
    void release();

private:
    std::unique_ptr<QTemporaryDir> m_tempDir;
    QStringList m_trackedFiles;
    bool m_released = false;
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_TEMPFILEGUARD_H
