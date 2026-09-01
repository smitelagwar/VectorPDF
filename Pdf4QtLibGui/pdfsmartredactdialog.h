// MIT License
#ifndef PDFSMARTREDACTDIALOG_H
#define PDFSMARTREDACTDIALOG_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QTableView;
class QPushButton;
class QCheckBox;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFSmartRedactDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFSmartRedactDialog(pdf::PDFDocument* document, QWidget* parent = nullptr);
    ~PDFSmartRedactDialog() override;

private slots:
    void onScanClicked();
    void onApplyPermanentlyClicked();

private:
    void initUi();

    QTableView* m_tableView = nullptr;
    QCheckBox* m_emailCheck = nullptr;
    QCheckBox* m_phoneCheck = nullptr;
    QCheckBox* m_tcCheck = nullptr;
    QCheckBox* m_ibanCheck = nullptr;
    QCheckBox* m_cardCheck = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFSMARTREDACTDIALOG_H
