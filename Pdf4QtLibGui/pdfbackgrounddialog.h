// MIT License
#ifndef PDFBACKGROUNDDIALOG_H
#define PDFBACKGROUNDDIALOG_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QComboBox;
class QLineEdit;
class QPushButton;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFBackgroundDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFBackgroundDialog(pdf::PDFDocument* document, QWidget* parent = nullptr);
    ~PDFBackgroundDialog() override;

private slots:
    void onApplyClicked();

private:
    void initUi();

    QComboBox* m_typeCombo = nullptr;
    QLineEdit* m_pathEdit = nullptr;
    QComboBox* m_placementCombo = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFBACKGROUNDDIALOG_H
