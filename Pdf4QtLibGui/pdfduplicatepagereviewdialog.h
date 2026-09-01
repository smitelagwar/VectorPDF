// MIT License
#ifndef PDFDUPLICATEPAGEREVIEWDIALOG_H
#define PDFDUPLICATEPAGEREVIEWDIALOG_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QTreeWidget;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFDuplicatePageReviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFDuplicatePageReviewDialog(pdf::PDFDocument* document, QWidget* parent = nullptr);
    ~PDFDuplicatePageReviewDialog() override;

private slots:
    void onDeduplicateClicked();

private:
    void initUi();

    QTreeWidget* m_duplicatesTree = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFDUPLICATEPAGEREVIEWDIALOG_H
