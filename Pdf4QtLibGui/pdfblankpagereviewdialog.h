// MIT License
#ifndef PDFBLANKPAGEREVIEWDIALOG_H
#define PDFBLANKPAGEREVIEWDIALOG_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QListWidget;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFBlankPageReviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFBlankPageReviewDialog(pdf::PDFDocument* document, QWidget* parent = nullptr);
    ~PDFBlankPageReviewDialog() override;

private slots:
    void onDeleteBlankPagesClicked();

private:
    void initUi();

    QListWidget* m_pagesList = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFBLANKPAGEREVIEWDIALOG_H
