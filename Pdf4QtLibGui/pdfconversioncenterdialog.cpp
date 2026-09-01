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

#include "pdfconversioncenterdialog.h"
#include "ui_pdfconversioncenterdialog.h"
#include "pdfscreenshotdialog.h"

#include <conversioncapability.h>
#include <utilities/filenamepolicy.h>
#include <utilities/tempfileguard.h>
#include <creators/imagepdfcreator.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include <QGuiApplication>
#include <QHeaderView>
#include <QProgressBar>

namespace vectorpdf::gui
{

PDFConversionCenterDialog::PDFConversionCenterDialog(const pdf::PDFDocument* activeDocument,
                                                     const QString& activeDocumentPath,
                                                     QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::PDFConversionCenterDialog)
    , m_activeDocument(activeDocument)
    , m_activeDocumentPath(activeDocumentPath)
{
    ui->setupUi(this);
    initUi();

    // Connect to ConversionService signals
    connect(&conversion::ConversionService::instance(), &conversion::ConversionService::jobEnqueued,
            this, &PDFConversionCenterDialog::onJobEnqueued);
    connect(&conversion::ConversionService::instance(), &conversion::ConversionService::jobStarted,
            this, &PDFConversionCenterDialog::onJobStarted);
    connect(&conversion::ConversionService::instance(), &conversion::ConversionService::jobProgress,
            this, &PDFConversionCenterDialog::onJobProgress);
    connect(&conversion::ConversionService::instance(), &conversion::ConversionService::jobFinished,
            this, &PDFConversionCenterDialog::onJobFinished);
}

PDFConversionCenterDialog::~PDFConversionCenterDialog()
{
    delete ui;
}

void PDFConversionCenterDialog::initUi()
{
    // Populate Export Formats
    ui->exportFormatCombo->addItem(QStringLiteral("Word Document (.docx)"), QVariant::fromValue(conversion::ConversionFormat::Docx));
    ui->exportFormatCombo->addItem(QStringLiteral("Excel Spreadsheet (.xlsx)"), QVariant::fromValue(conversion::ConversionFormat::Xlsx));
    ui->exportFormatCombo->addItem(QStringLiteral("PowerPoint Presentation (.pptx)"), QVariant::fromValue(conversion::ConversionFormat::Pptx));
    ui->exportFormatCombo->addItem(QStringLiteral("PNG Image (.png)"), QVariant::fromValue(conversion::ConversionFormat::Png));
    ui->exportFormatCombo->addItem(QStringLiteral("JPEG Image (.jpg)"), QVariant::fromValue(conversion::ConversionFormat::Jpeg));
    ui->exportFormatCombo->addItem(QStringLiteral("TIFF Image (.tiff)"), QVariant::fromValue(conversion::ConversionFormat::Tiff));
    ui->exportFormatCombo->addItem(QStringLiteral("WebP Image (.webp)"), QVariant::fromValue(conversion::ConversionFormat::WebP));
    ui->exportFormatCombo->addItem(QStringLiteral("BMP Image (.bmp)"), QVariant::fromValue(conversion::ConversionFormat::Bmp));
    ui->exportFormatCombo->addItem(QStringLiteral("PDF/A-1b Archival"), QVariant::fromValue(conversion::ConversionFormat::PdfA1));
    ui->exportFormatCombo->addItem(QStringLiteral("PDF/A-2b Archival"), QVariant::fromValue(conversion::ConversionFormat::PdfA2));
    ui->exportFormatCombo->addItem(QStringLiteral("PDF/A-3b Archival"), QVariant::fromValue(conversion::ConversionFormat::PdfA3));
    ui->exportFormatCombo->addItem(QStringLiteral("PDF/A-4 Archival"), QVariant::fromValue(conversion::ConversionFormat::PdfA4));
    ui->exportFormatCombo->addItem(QStringLiteral("Monochrome PDF (Bilevel)"), QVariant::fromValue(conversion::ConversionFormat::MonochromePdf));
    ui->exportFormatCombo->addItem(QStringLiteral("Form Data XFDF (.xfdf)"), QVariant::fromValue(conversion::ConversionFormat::Xfdf));
    ui->exportFormatCombo->addItem(QStringLiteral("Form Data FDF (.fdf)"), QVariant::fromValue(conversion::ConversionFormat::Fdf));

    // Populate Create Source Types
    ui->createSourceTypeCombo->addItem(QStringLiteral("Single Document / Image File"), 0);
    ui->createSourceTypeCombo->addItem(QStringLiteral("Multiple Images (Combine)"), 1);
    ui->createSourceTypeCombo->addItem(QStringLiteral("From System Clipboard"), 2);
    ui->createSourceTypeCombo->addItem(QStringLiteral("From Screen Capture"), 3);
    ui->createSourceTypeCombo->addItem(QStringLiteral("From Folder (Batch Merge)"), 4);

    if (!m_activeDocumentPath.isEmpty())
    {
        ui->exportSourcePathEdit->setText(m_activeDocumentPath);
        ui->exportOutputPathEdit->setText(conversion::FilenamePolicy::computeDefaultOutputPath(m_activeDocumentPath, conversion::ConversionFormat::Docx));
    }

    ui->queueTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->queueTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->queueTableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);

    // Wire Export connections
    connect(ui->exportFormatCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PDFConversionCenterDialog::onExportFormatChanged);
    connect(ui->exportSourceBrowseButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onExportSourceBrowse);
    connect(ui->exportOutputBrowseButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onExportTargetBrowse);
    connect(ui->exportStartButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onStartExport);

    // Wire Create connections
    connect(ui->createSourceTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PDFConversionCenterDialog::onCreateSourceTypeChanged);
    connect(ui->createSingleFileBrowseButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onCreateSourceBrowse);
    connect(ui->createAddImagesButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onCreateAddImages);
    connect(ui->createRemoveImageButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onCreateRemoveImage);
    connect(ui->createClearImagesButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onCreateClearImages);
    connect(ui->createClipboardPasteButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onCreatePasteClipboard);
    connect(ui->createCaptureScreenButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onCreateCaptureScreenshot);
    connect(ui->createCaptureRegionButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onCreateCaptureScreenshot);
    connect(ui->createFolderBrowseButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onCreateSourceBrowse);
    connect(ui->createOutputBrowseButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onCreateTargetBrowse);
    connect(ui->createStartButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onStartCreate);

    // Wire Queue connections
    connect(ui->queueCancelButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onCancelSelectedJob);
    connect(ui->queueOpenFileButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onOpenFileSelected);
    connect(ui->queueOpenFolderButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onOpenFolderSelected);
    connect(ui->queueClearFinishedButton, &QPushButton::clicked, this, &PDFConversionCenterDialog::onClearFinishedJobs);

    onExportFormatChanged(0);
}

void PDFConversionCenterDialog::setCurrentTab(int tabIndex)
{
    ui->tabWidget->setCurrentIndex(tabIndex);
}

void PDFConversionCenterDialog::setExportFormat(conversion::ConversionFormat format)
{
    for (int i = 0; i < ui->exportFormatCombo->count(); ++i)
    {
        if (ui->exportFormatCombo->itemData(i).value<conversion::ConversionFormat>() == format)
        {
            ui->exportFormatCombo->setCurrentIndex(i);
            break;
        }
    }
}

void PDFConversionCenterDialog::onExportFormatChanged(int index)
{
    conversion::ConversionFormat format = ui->exportFormatCombo->itemData(index).value<conversion::ConversionFormat>();

    ui->exportModeCombo->clear();

    if (format == conversion::ConversionFormat::Docx)
    {
        ui->exportModeCombo->addItem(QStringLiteral("Reflow (Editable Text)"), 0);
        ui->exportModeCombo->addItem(QStringLiteral("Visual Fidelity (Exact Page Image)"), 1);
        ui->exportModeCombo->setEnabled(true);
        ui->exportDpiSpin->setEnabled(true);
    }
    else if (format == conversion::ConversionFormat::Xlsx)
    {
        ui->exportModeCombo->addItem(QStringLiteral("Detected Tables Only"), 0);
        ui->exportModeCombo->addItem(QStringLiteral("Full Page Layout Grid"), 1);
        ui->exportModeCombo->setEnabled(true);
        ui->exportDpiSpin->setEnabled(false);
    }
    else if (format == conversion::ConversionFormat::Pptx)
    {
        ui->exportModeCombo->addItem(QStringLiteral("Visual Slide per Page"), 0);
        ui->exportModeCombo->addItem(QStringLiteral("Editable Text Boxes Preview"), 1);
        ui->exportModeCombo->setEnabled(true);
        ui->exportDpiSpin->setEnabled(true);
    }
    else if (format == conversion::ConversionFormat::MonochromePdf)
    {
        ui->exportModeCombo->addItem(QStringLiteral("Otsu Auto Threshold (Default)"), 0);
        ui->exportModeCombo->addItem(QStringLiteral("Fixed Threshold 128"), 1);
        ui->exportModeCombo->setEnabled(true);
        ui->exportDpiSpin->setEnabled(true);
    }
    else if (format == conversion::ConversionFormat::Tiff)
    {
        ui->exportModeCombo->addItem(QStringLiteral("Multi-Page TIFF File"), 0);
        ui->exportModeCombo->addItem(QStringLiteral("Single Page per TIFF File"), 1);
        ui->exportModeCombo->setEnabled(true);
        ui->exportDpiSpin->setEnabled(true);
    }
    else
    {
        ui->exportModeCombo->addItem(QStringLiteral("Default"), 0);
        ui->exportModeCombo->setEnabled(false);
        ui->exportDpiSpin->setEnabled(format == conversion::ConversionFormat::Png ||
                                      format == conversion::ConversionFormat::Jpeg ||
                                      format == conversion::ConversionFormat::WebP ||
                                      format == conversion::ConversionFormat::Bmp);
    }

    const QString src = ui->exportSourcePathEdit->text();
    if (!src.isEmpty())
    {
        ui->exportOutputPathEdit->setText(conversion::FilenamePolicy::computeDefaultOutputPath(src, format));
    }

    updateExportCapabilityInfo();
}

void PDFConversionCenterDialog::updateExportCapabilityInfo()
{
    conversion::ConversionFormat format = ui->exportFormatCombo->currentData().value<conversion::ConversionFormat>();
    auto info = conversion::ConversionCapability::instance().getExportCapability(format);

    QString text = QStringLiteral("<b>Engine:</b> %1 — %2").arg(info.engineName).arg(info.note);
    if (info.isLossy)
    {
        text.append(QStringLiteral(" <span style='color: #d97706;'>(Lossy/Rasterized)</span>"));
    }
    ui->exportCapabilityLabel->setText(text);
}

void PDFConversionCenterDialog::onExportSourceBrowse()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select Source PDF"), QString(), tr("PDF Files (*.pdf);;All Files (*.*)"));
    if (!path.isEmpty())
    {
        ui->exportSourcePathEdit->setText(path);
        conversion::ConversionFormat format = ui->exportFormatCombo->currentData().value<conversion::ConversionFormat>();
        ui->exportOutputPathEdit->setText(conversion::FilenamePolicy::computeDefaultOutputPath(path, format));
    }
}

void PDFConversionCenterDialog::onExportTargetBrowse()
{
    conversion::ConversionFormat format = ui->exportFormatCombo->currentData().value<conversion::ConversionFormat>();
    QString ext = conversion::formatDefaultExtension(format);
    QString filter = QStringLiteral("%1 Files (*.%2);;All Files (*.*)").arg(ext.toUpper()).arg(ext);

    QString path = QFileDialog::getSaveFileName(this, tr("Select Output Destination"), ui->exportOutputPathEdit->text(), filter);
    if (!path.isEmpty())
    {
        ui->exportOutputPathEdit->setText(path);
    }
}

void PDFConversionCenterDialog::onStartExport()
{
    const QString src = ui->exportSourcePathEdit->text().trimmed();
    if (src.isEmpty() || !QFile::exists(src))
    {
        QMessageBox::warning(this, tr("Invalid Source"), tr("Please specify a valid source PDF file."));
        return;
    }

    conversion::ConversionRequest req;
    req.sourcePath = src;
    req.outputPath = ui->exportOutputPathEdit->text().trimmed();
    req.format = ui->exportFormatCombo->currentData().value<conversion::ConversionFormat>();
    req.pageRangeString = ui->exportPagesEdit->text().trimmed();
    req.dpi = ui->exportDpiSpin->value();

    if (req.format == conversion::ConversionFormat::Docx)
    {
        req.docxMode = ui->exportModeCombo->currentIndex() == 1 ? conversion::DocxExportMode::Visual : conversion::DocxExportMode::Reflow;
    }
    else if (req.format == conversion::ConversionFormat::Xlsx)
    {
        req.xlsxMode = ui->exportModeCombo->currentIndex() == 1 ? conversion::XlsxExportMode::PageLayout : conversion::XlsxExportMode::DetectedTables;
    }
    else if (req.format == conversion::ConversionFormat::Pptx)
    {
        req.pptxMode = ui->exportModeCombo->currentIndex() == 1 ? conversion::PptxExportMode::EditablePreview : conversion::PptxExportMode::Visual;
    }
    else if (req.format == conversion::ConversionFormat::MonochromePdf)
    {
        req.monochromeMethod = ui->exportModeCombo->currentIndex() == 1 ? conversion::MonochromeThresholdMethod::Fixed : conversion::MonochromeThresholdMethod::Otsu;
    }
    else if (req.format == conversion::ConversionFormat::Tiff)
    {
        req.multiPageTiff = (ui->exportModeCombo->currentIndex() == 0);
    }

    conversion::ConversionService::instance().enqueue(req);
    ui->tabWidget->setCurrentIndex(2); // switch to Queue tab
}

void PDFConversionCenterDialog::onCreateSourceTypeChanged(int index)
{
    ui->createStack->setCurrentIndex(index);
}

void PDFConversionCenterDialog::onCreateSourceBrowse()
{
    int type = ui->createSourceTypeCombo->currentIndex();
    if (type == 0) // Single file
    {
        QString path = QFileDialog::getOpenFileName(this, tr("Select Source File"), QString(),
            tr("All Supported Files (*.docx *.doc *.html *.htm *.md *.markdown *.png *.jpg *.jpeg *.tif *.tiff *.webp *.bmp);;Word Files (*.docx *.doc);;HTML Files (*.html *.htm);;Markdown Files (*.md *.markdown);;Images (*.png *.jpg *.jpeg *.tif *.tiff *.webp *.bmp);;All Files (*.*)"));
        if (!path.isEmpty())
        {
            ui->createSingleFilePathEdit->setText(path);
            ui->createOutputPathEdit->setText(conversion::FilenamePolicy::computeDefaultOutputPath(path, conversion::ConversionFormat::Pdf));
        }
    }
    else if (type == 4) // Folder
    {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Select Folder to Convert to PDF"));
        if (!dir.isEmpty())
        {
            ui->createFolderPathEdit->setText(dir);
            ui->createOutputPathEdit->setText(QDir(dir).filePath(QFileInfo(dir).fileName() + QStringLiteral(".pdf")));
        }
    }
}

void PDFConversionCenterDialog::onCreateAddImages()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Add Images to Combine"), QString(),
        tr("Images (*.png *.jpg *.jpeg *.tif *.tiff *.webp *.bmp);;All Files (*.*)"));
    for (const QString& file : files)
    {
        if (!m_createSelectedImageFiles.contains(file))
        {
            m_createSelectedImageFiles.append(file);
            ui->createImageListWidget->addItem(QFileInfo(file).fileName());
        }
    }
    if (!m_createSelectedImageFiles.isEmpty() && ui->createOutputPathEdit->text().isEmpty())
    {
        ui->createOutputPathEdit->setText(conversion::FilenamePolicy::computeDefaultOutputPath(m_createSelectedImageFiles.first(), conversion::ConversionFormat::Pdf));
    }
}

void PDFConversionCenterDialog::onCreateRemoveImage()
{
    int row = ui->createImageListWidget->currentRow();
    if (row >= 0 && row < m_createSelectedImageFiles.size())
    {
        m_createSelectedImageFiles.removeAt(row);
        delete ui->createImageListWidget->takeItem(row);
    }
}

void PDFConversionCenterDialog::onCreateClearImages()
{
    m_createSelectedImageFiles.clear();
    ui->createImageListWidget->clear();
}

void PDFConversionCenterDialog::onCreatePasteClipboard()
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    QImage img = clipboard->image();
    if (img.isNull())
    {
        QMessageBox::information(this, tr("Clipboard Empty"), tr("No image found on the system clipboard."));
        return;
    }

    conversion::TempFileGuard tempGuard(QStringLiteral("vpdf_clip_"));
    QString tempPng = tempGuard.createTempFilePath(QStringLiteral("png"));
    img.save(tempPng, "PNG");
    tempGuard.release();

    ui->createSourceTypeCombo->setCurrentIndex(0);
    ui->createSingleFilePathEdit->setText(tempPng);
    ui->createOutputPathEdit->setText(QDir::homePath() + QStringLiteral("/Clipboard_Document.pdf"));
}

void PDFConversionCenterDialog::onCreateCaptureScreenshot()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    bool isRegion = (btn == ui->createCaptureRegionButton);

    QImage img = isRegion ? PDFScreenshotDialog::captureRegion(this)
                          : PDFScreenshotDialog::captureFullScreen();

    if (img.isNull()) return;

    conversion::TempFileGuard tempGuard(QStringLiteral("vpdf_shot_"));
    QString tempPng = tempGuard.createTempFilePath(QStringLiteral("png"));
    img.save(tempPng, "PNG");
    tempGuard.release();

    ui->createSourceTypeCombo->setCurrentIndex(0);
    ui->createSingleFilePathEdit->setText(tempPng);
    ui->createOutputPathEdit->setText(QDir::homePath() + QStringLiteral("/Screenshot_Document.pdf"));
}

void PDFConversionCenterDialog::onCreateTargetBrowse()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Select Target PDF Destination"), ui->createOutputPathEdit->text(), tr("PDF Files (*.pdf);;All Files (*.*)"));
    if (!path.isEmpty())
    {
        ui->createOutputPathEdit->setText(path);
    }
}

void PDFConversionCenterDialog::onStartCreate()
{
    int type = ui->createSourceTypeCombo->currentIndex();
    conversion::ConversionRequest req;
    req.format = conversion::ConversionFormat::Pdf;
    req.outputPath = ui->createOutputPathEdit->text().trimmed();

    if (req.outputPath.isEmpty())
    {
        QMessageBox::warning(this, tr("Invalid Destination"), tr("Please specify a valid target PDF path."));
        return;
    }

    if (type == 0) // Single file
    {
        req.sourcePath = ui->createSingleFilePathEdit->text().trimmed();
        if (req.sourcePath.isEmpty() || !QFile::exists(req.sourcePath))
        {
            QMessageBox::warning(this, tr("Invalid Source"), tr("Please specify a valid source file."));
            return;
        }
    }
    else if (type == 1) // Multiple images
    {
        if (m_createSelectedImageFiles.isEmpty())
        {
            QMessageBox::warning(this, tr("No Images"), tr("Please add at least one image to combine into PDF."));
            return;
        }
        req.sourcePaths = m_createSelectedImageFiles;
    }
    else if (type == 4) // Folder
    {
        req.sourcePath = ui->createFolderPathEdit->text().trimmed();
        if (req.sourcePath.isEmpty() || !QDir(req.sourcePath).exists())
        {
            QMessageBox::warning(this, tr("Invalid Folder"), tr("Please specify a valid source directory."));
            return;
        }
        req.recursiveFolder = ui->createFolderRecursiveCheck->isChecked();
        req.createBookmarksFromFilenames = ui->createFolderBookmarksCheck->isChecked();
    }

    conversion::ConversionService::instance().enqueue(req);
    ui->tabWidget->setCurrentIndex(2); // switch to Queue tab
}

void PDFConversionCenterDialog::addJobToTable(conversion::ConversionJob* job)
{
    if (!job) return;

    int row = ui->queueTableWidget->rowCount();
    ui->queueTableWidget->insertRow(row);

    QTableWidgetItem* idItem = new QTableWidgetItem(job->request().jobId.left(8));
    idItem->setData(Qt::UserRole, job->request().jobId);
    ui->queueTableWidget->setItem(row, 0, idItem);

    QString srcName = job->request().sourcePath.isEmpty()
                          ? QStringLiteral("%1 images").arg(job->request().sourcePaths.size())
                          : QFileInfo(job->request().sourcePath).fileName();
    ui->queueTableWidget->setItem(row, 1, new QTableWidgetItem(srcName));

    ui->queueTableWidget->setItem(row, 2, new QTableWidgetItem(conversion::formatToString(job->request().format)));
    ui->queueTableWidget->setItem(row, 3, new QTableWidgetItem(conversion::statusToString(job->status())));

    QProgressBar* bar = new QProgressBar();
    bar->setRange(0, 100);
    bar->setValue(job->progressPercentage());
    ui->queueTableWidget->setCellWidget(row, 4, bar);

    ui->queueTableWidget->setItem(row, 5, new QTableWidgetItem(job->request().outputPath));
}

int PDFConversionCenterDialog::findJobRow(const QString& jobId)
{
    for (int r = 0; r < ui->queueTableWidget->rowCount(); ++r)
    {
        QTableWidgetItem* item = ui->queueTableWidget->item(r, 0);
        if (item && item->data(Qt::UserRole).toString() == jobId)
        {
            return r;
        }
    }
    return -1;
}

void PDFConversionCenterDialog::onJobEnqueued(conversion::ConversionJob* job)
{
    addJobToTable(job);
}

void PDFConversionCenterDialog::onJobStarted(conversion::ConversionJob* job)
{
    int row = findJobRow(job->request().jobId);
    if (row >= 0)
    {
        ui->queueTableWidget->setItem(row, 3, new QTableWidgetItem(conversion::statusToString(job->status())));
    }
}

void PDFConversionCenterDialog::onJobProgress(conversion::ConversionJob* job, int percentage, const QString& message)
{
    Q_UNUSED(message);
    int row = findJobRow(job->request().jobId);
    if (row >= 0)
    {
        QProgressBar* bar = qobject_cast<QProgressBar*>(ui->queueTableWidget->cellWidget(row, 4));
        if (bar)
        {
            bar->setValue(percentage);
        }
        ui->queueTableWidget->setItem(row, 3, new QTableWidgetItem(conversion::stageToString(job->stage())));
    }
}

void PDFConversionCenterDialog::onJobFinished(conversion::ConversionJob* job, const conversion::ConversionResult& result)
{
    int row = findJobRow(job->request().jobId);
    if (row >= 0)
    {
        ui->queueTableWidget->setItem(row, 3, new QTableWidgetItem(conversion::statusToString(result.status)));
        QProgressBar* bar = qobject_cast<QProgressBar*>(ui->queueTableWidget->cellWidget(row, 4));
        if (bar)
        {
            bar->setValue(result.isSuccess() ? 100 : bar->value());
        }
        ui->queueTableWidget->setItem(row, 5, new QTableWidgetItem(result.outputPath));
    }
}

void PDFConversionCenterDialog::onCancelSelectedJob()
{
    int row = ui->queueTableWidget->currentRow();
    if (row >= 0)
    {
        QTableWidgetItem* item = ui->queueTableWidget->item(row, 0);
        if (item)
        {
            QString jobId = item->data(Qt::UserRole).toString();
            conversion::ConversionService::instance().cancelJob(jobId);
        }
    }
}

void PDFConversionCenterDialog::onOpenFileSelected()
{
    int row = ui->queueTableWidget->currentRow();
    if (row >= 0)
    {
        QTableWidgetItem* item = ui->queueTableWidget->item(row, 5);
        if (item && QFile::exists(item->text()))
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(item->text()));
        }
    }
}

void PDFConversionCenterDialog::onOpenFolderSelected()
{
    int row = ui->queueTableWidget->currentRow();
    if (row >= 0)
    {
        QTableWidgetItem* item = ui->queueTableWidget->item(row, 5);
        if (item && !item->text().isEmpty())
        {
            QFileInfo info(item->text());
            QDesktopServices::openUrl(QUrl::fromLocalFile(info.dir().path()));
        }
    }
}

void PDFConversionCenterDialog::onClearFinishedJobs()
{
    for (int r = ui->queueTableWidget->rowCount() - 1; r >= 0; --r)
    {
        QTableWidgetItem* statusItem = ui->queueTableWidget->item(r, 3);
        if (statusItem)
        {
            const QString st = statusItem->text();
            if (st == conversion::statusToString(conversion::ConversionStatus::Success) ||
                st == conversion::statusToString(conversion::ConversionStatus::SuccessWithWarnings) ||
                st == conversion::statusToString(conversion::ConversionStatus::Failed) ||
                st == conversion::statusToString(conversion::ConversionStatus::Cancelled))
            {
                ui->queueTableWidget->removeRow(r);
            }
        }
    }
}

} // namespace vectorpdf::gui
