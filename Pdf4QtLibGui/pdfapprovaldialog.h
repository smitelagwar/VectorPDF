// MIT License
#ifndef PDFAPPROVALDIALOG_H
#define PDFAPPROVALDIALOG_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QLineEdit;
class QListWidget;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFApprovalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFApprovalDialog(pdf::PDFDocument* document, QWidget* parent = nullptr);
    ~PDFApprovalDialog() override;

private slots:
    void onSendApprovalClicked();

private:
    void initUi();

    QLineEdit* m_titleEdit = nullptr;
    QListWidget* m_approversList = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFAPPROVALDIALOG_H
