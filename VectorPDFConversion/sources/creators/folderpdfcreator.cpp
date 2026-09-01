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

#include "folderpdfcreator.h"
#include "imagepdfcreator.h"
#include "markdownpdfcreator.h"
#include "htmlpdfcreator.h"
#include "officepdfcreator.h"
#include "../utilities/tempfileguard.h"
#include "../utilities/filenamepolicy.h"
#include "../utilities/mimedetector.h"
#include "../utilities/outputverification.h"

#include <pdfdocument.h>
#include <pdfdocumentreader.h>
#include <pdfdocumentwriter.h>
#include <pdfdocumentmanipulator.h>

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QElapsedTimer>
#include <memory>

namespace vectorpdf::conversion
{

QList<FolderItemInfo> FolderPdfCreator::scanFolder(const QString& folderPath, bool recursive)
{
    QList<FolderItemInfo> items;
    QDir rootDir(folderPath);
    if (!rootDir.exists()) return items;

    QDirIterator::IteratorFlags flags = QDirIterator::NoIteratorFlags;
    if (recursive)
    {
        flags = QDirIterator::Subdirectories;
    }

    QDirIterator it(folderPath, QDir::Files | QDir::Readable, flags);
    while (it.hasNext())
    {
        const QString path = it.next();
        FileKind kind = MimeDetector::detectFileKind(path);
        if (kind != FileKind::Unknown)
        {
            FolderItemInfo info;
            info.filePath = path;
            info.fileName = it.fileName();
            info.relativePath = rootDir.relativeFilePath(path);
            info.kind = kind;
            info.sizeBytes = it.fileInfo().size();
            items.append(info);
        }
    }

    // Natural sort by relative path
    std::sort(items.begin(), items.end(), [](const FolderItemInfo& a, const FolderItemInfo& b) {
        return QString::compare(a.relativePath, b.relativePath, Qt::CaseInsensitive) < 0;
    });

    return items;
}

ConversionResult FolderPdfCreator::execute(const ConversionRequest& request,
                                           const ProgressCallback& progress,
                                           CancelToken* cancelToken)
{
    QElapsedTimer timer;
    timer.start();

    const QString folderPath = request.sourcePath;
    if (!QDir(folderPath).exists())
    {
        return ConversionResult::failure(QStringLiteral("FolderNotFound"),
                                         QStringLiteral("Source folder does not exist: '%1'").arg(folderPath));
    }

    if (progress) progress(ConversionStage::Loading, 5, QStringLiteral("Scanning folder for convertible files..."));

    QList<FolderItemInfo> items = scanFolder(folderPath, request.recursiveFolder);
    if (items.isEmpty())
    {
        return ConversionResult::failure(QStringLiteral("NoConvertibleFiles"),
                                         QStringLiteral("No supported convertible files found in folder: '%1'").arg(folderPath));
    }

    TempFileGuard tempGuard(QStringLiteral("vpdf_fld_"));
    if (!tempGuard.isValid())
    {
        return ConversionResult::failure(QStringLiteral("TempDirError"), QStringLiteral("Failed to create temporary directory."));
    }

    QStringList normalizedPdfPaths;
    QStringList skippedWarnings;
    const int totalItems = items.size();

    // 1. Convert each file into a normalized PDF
    for (int i = 0; i < totalItems; ++i)
    {
        if (cancelToken && cancelToken->load()) return ConversionResult::cancelled();

        const FolderItemInfo& item = items[i];
        const QString tempPdf = tempGuard.createTempFilePath(QStringLiteral("pdf"), QStringLiteral("norm_"));

        bool converted = false;
        QString itemErr;

        if (item.kind == FileKind::Pdf)
        {
            // Already PDF: verify and use
            if (OutputVerification::verifyPdf(item.filePath, 0, &itemErr))
            {
                normalizedPdfPaths.append(item.filePath);
                converted = true;
            }
        }
        else if (item.kind == FileKind::Image)
        {
            ConversionRequest imgReq;
            imgReq.sourcePath = item.filePath;
            imgReq.outputPath = tempPdf;
            imgReq.dpi = request.dpi > 0 ? request.dpi : 300;

            ImagePdfCreator imgCreator;
            ConversionResult r = imgCreator.execute(imgReq, nullptr, cancelToken);
            if (r.isSuccess() && QFile::exists(tempPdf))
            {
                normalizedPdfPaths.append(tempPdf);
                converted = true;
            }
            else
            {
                itemErr = r.safeMessage;
            }
        }
        else if (item.kind == FileKind::Markdown)
        {
            ConversionRequest mdReq;
            mdReq.sourcePath = item.filePath;
            mdReq.outputPath = tempPdf;
            mdReq.markdownTheme = request.markdownTheme;

            MarkdownPdfCreator mdCreator;
            ConversionResult r = mdCreator.execute(mdReq, nullptr, cancelToken);
            if (r.isSuccess() && QFile::exists(tempPdf))
            {
                normalizedPdfPaths.append(tempPdf);
                converted = true;
            }
            else
            {
                itemErr = r.safeMessage;
            }
        }
        else if (item.kind == FileKind::Html)
        {
            ConversionRequest htmlReq;
            htmlReq.sourcePath = item.filePath;
            htmlReq.outputPath = tempPdf;

            HtmlPdfCreator htmlCreator;
            ConversionResult r = htmlCreator.execute(htmlReq, nullptr, cancelToken);
            if (r.isSuccess() && QFile::exists(tempPdf))
            {
                normalizedPdfPaths.append(tempPdf);
                converted = true;
            }
            else
            {
                itemErr = r.safeMessage;
            }
        }
        else if (item.kind == FileKind::OfficeDocument)
        {
            ConversionRequest offReq;
            offReq.sourcePath = item.filePath;
            offReq.outputPath = tempPdf;

            OfficePdfCreator offCreator;
            ConversionResult r = offCreator.execute(offReq, nullptr, cancelToken);
            if (r.isSuccess() && QFile::exists(tempPdf))
            {
                normalizedPdfPaths.append(tempPdf);
                converted = true;
            }
            else
            {
                itemErr = r.safeMessage;
            }
        }

        if (!converted)
        {
            if (request.stopOnError)
            {
                return ConversionResult::failure(QStringLiteral("FileConversionFailed"),
                                                 QStringLiteral("Failed to convert '%1': %2").arg(item.fileName).arg(itemErr));
            }
            else
            {
                skippedWarnings.append(QStringLiteral("Skipped '%1': %2").arg(item.fileName).arg(itemErr));
            }
        }

        if (progress)
        {
            int p = 10 + ((i + 1) * 60) / totalItems;
            progress(ConversionStage::Rendering, p, QStringLiteral("Converted %1 of %2 items").arg(i + 1).arg(totalItems));
        }
    }

    if (normalizedPdfPaths.isEmpty())
    {
        return ConversionResult::failure(QStringLiteral("NoPdfsProduced"),
                                         QStringLiteral("No items could be successfully converted to PDF."));
    }

    // 2. Merge all normalized PDFs
    if (progress) progress(ConversionStage::Encoding, 75, QStringLiteral("Merging all pages into single PDF..."));

    struct LoadedDoc
    {
        QByteArray data;
        pdf::PDFDocument doc;
    };

    std::vector<std::unique_ptr<LoadedDoc>> loadedDocs;
    pdf::PDFDocumentManipulator manipulator;
    if (request.createBookmarksFromFilenames)
    {
        manipulator.setOutlineMode(pdf::PDFDocumentManipulator::OutlineMode::DocumentParts);
    }

    pdf::PDFDocumentManipulator::AssembledPages assembledPages;

    for (int i = 0; i < normalizedPdfPaths.size(); ++i)
    {
        if (cancelToken && cancelToken->load()) return ConversionResult::cancelled();

        const QString& pdfPath = normalizedPdfPaths[i];
        QFile f(pdfPath);
        if (!f.open(QIODevice::ReadOnly)) continue;

        auto ld = std::make_unique<LoadedDoc>();
        ld->data = f.readAll();
        f.close();

        pdf::PDFDocumentReader reader(&ld->doc);
        if (!reader.read(ld->data).isSuccessful() || !ld->doc.getCatalog()) continue;

        int docIdx = static_cast<int>(loadedDocs.size());
        manipulator.addDocument(docIdx, &ld->doc);

        auto docPages = pdf::PDFDocumentManipulator::createAllDocumentPages(docIdx, &ld->doc);
        for (const auto& dp : docPages)
        {
            assembledPages.push_back(dp);
        }

        loadedDocs.push_back(std::move(ld));
    }

    if (assembledPages.empty())
    {
        return ConversionResult::failure(QStringLiteral("AssembleFailed"), QStringLiteral("Failed to assemble PDF pages from documents."));
    }

    auto assembleRes = manipulator.assemble(assembledPages);
    if (!assembleRes.isSuccessful())
    {
        return ConversionResult::failure(QStringLiteral("AssembleFailed"),
                                         QStringLiteral("PDF assemble failed: %1").arg(assembleRes.getMessage()));
    }

    pdf::PDFDocument assembledDoc = manipulator.takeAssembledDocument();

    const QString tempFinalPdf = tempGuard.createTempFilePath(QStringLiteral("pdf"), QStringLiteral("final_"));
    pdf::PDFDocumentWriter writer(nullptr);
    auto writeRes = writer.write(tempFinalPdf, &assembledDoc, true);
    if (!writeRes.isSuccessful())
    {
        return ConversionResult::failure(QStringLiteral("WriteFailed"),
                                         QStringLiteral("Failed to write final merged PDF: %1").arg(writeRes.getMessage()));
    }

    if (progress) progress(ConversionStage::Validating, 90, QStringLiteral("Validating final PDF..."));

    QString verifyErr;
    if (!OutputVerification::verifyPdf(tempFinalPdf, 0, &verifyErr))
    {
        return ConversionResult::failure(QStringLiteral("ValidationFailed"), QStringLiteral("Final PDF validation failed: %1").arg(verifyErr));
    }

    QString finalOutPath = request.outputPath.isEmpty()
                               ? QDir(folderPath).filePath(QFileInfo(folderPath).fileName() + QStringLiteral(".pdf"))
                               : request.outputPath;
    finalOutPath = FilenamePolicy::resolveCollision(finalOutPath, request.overwriteExisting);

    if (progress) progress(ConversionStage::Committing, 95, QStringLiteral("Committing final PDF..."));

    QString commitErr;
    if (!TempFileGuard::atomicCommit(tempFinalPdf, finalOutPath, &commitErr))
    {
        return ConversionResult::failure(QStringLiteral("CommitFailed"), QStringLiteral("Failed to commit final PDF: %1").arg(commitErr));
    }

    if (progress) progress(ConversionStage::Completed, 100, QStringLiteral("Folder conversion completed."));

    ConversionResult res = ConversionResult::success(finalOutPath);
    res.status = skippedWarnings.isEmpty() ? ConversionStatus::Success : ConversionStatus::SuccessWithWarnings;
    res.warnings = skippedWarnings;
    res.totalPagesProcessed = static_cast<int>(assembledPages.size());
    res.elapsedMilliseconds = timer.elapsed();
    res.outputSizeBytes = QFileInfo(finalOutPath).size();
    return res;
}

} // namespace vectorpdf::conversion
