// MIT License
#ifndef PDFSIGNREQUESTDIALOG_H
#define PDFSIGNREQUESTDIALOG_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QLineEdit;
class QListWidget;
class QPushButton;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFSignRequestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFSignRequestDialog(pdf::PDFDocument* document, QWidget* parent = nullptr);
    ~PDFSignRequestDialog() override;

private slots:
    void onAddSignerClicked();
    void onSendRequestClicked();

private:
    void initUi();

    QLineEdit* m_titleEdit = nullptr;
    QLineEdit* m_emailEdit = nullptr;
    QLineEdit* m_nameEdit = nullptr;
    QListWidget* m_signersList = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFSIGNREQUESTDIALOG_H
