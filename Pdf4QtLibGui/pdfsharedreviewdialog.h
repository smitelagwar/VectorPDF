// MIT License
#ifndef PDFSHAREDREVIEWDIALOG_H
#define PDFSHAREDREVIEWDIALOG_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QLineEdit;
class QComboBox;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFSharedReviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFSharedReviewDialog(pdf::PDFDocument* document, QWidget* parent = nullptr);
    ~PDFSharedReviewDialog() override;

private slots:
    void onStartReviewClicked();

private:
    void initUi();

    QLineEdit* m_titleEdit = nullptr;
    QComboBox* m_storageCombo = nullptr;
    QLineEdit* m_pathEdit = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFSHAREDREVIEWDIALOG_H
