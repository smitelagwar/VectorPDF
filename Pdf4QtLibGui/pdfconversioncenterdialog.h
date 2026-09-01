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

#ifndef VECTORPDF_PDFCONVERSIONCENTERDIALOG_H
#define VECTORPDF_PDFCONVERSIONCENTERDIALOG_H

#include <QDialog>
#include <QList>
#include <conversiontypes.h>
#include <conversionrequest.h>
#include <conversionresult.h>
#include <conversionservice.h>

namespace Ui
{
class PDFConversionCenterDialog;
}

namespace pdf
{
class PDFDocument;
}

namespace vectorpdf::gui
{

class PDFConversionCenterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFConversionCenterDialog(const pdf::PDFDocument* activeDocument = nullptr,
                                       const QString& activeDocumentPath = QString(),
                                       QWidget* parent = nullptr);
    virtual ~PDFConversionCenterDialog() override;

    /// Sets the active tab (0 = Export, 1 = Create, 2 = Queue)
    void setCurrentTab(int tabIndex);

    /// Pre-selects export format
    void setExportFormat(conversion::ConversionFormat format);

private slots:
    // Export tab slots
    void onExportFormatChanged(int index);
    void onExportSourceBrowse();
    void onExportTargetBrowse();
    void onStartExport();

    // Create tab slots
    void onCreateSourceTypeChanged(int index);
    void onCreateSourceBrowse();
    void onCreateAddImages();
    void onCreateRemoveImage();
    void onCreateClearImages();
    void onCreatePasteClipboard();
    void onCreateCaptureScreenshot();
    void onCreateTargetBrowse();
    void onStartCreate();

    // Queue tab slots
    void onJobEnqueued(conversion::ConversionJob* job);
    void onJobStarted(conversion::ConversionJob* job);
    void onJobProgress(conversion::ConversionJob* job, int percentage, const QString& message);
    void onJobFinished(conversion::ConversionJob* job, const conversion::ConversionResult& result);
    void onCancelSelectedJob();
    void onOpenFileSelected();
    void onOpenFolderSelected();
    void onClearFinishedJobs();

private:
    void initUi();
    void updateExportCapabilityInfo();
    void addJobToTable(conversion::ConversionJob* job);
    int findJobRow(const QString& jobId);

    Ui::PDFConversionCenterDialog* ui;
    const pdf::PDFDocument* m_activeDocument = nullptr;
    QString m_activeDocumentPath;
    QStringList m_createSelectedImageFiles;
};

} // namespace vectorpdf::gui

#endif // VECTORPDF_PDFCONVERSIONCENTERDIALOG_H
