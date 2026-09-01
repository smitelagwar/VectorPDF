// MIT License
#ifndef PDFBATESDIALOG_H
#define PDFBATESDIALOG_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QLineEdit;
class QSpinBox;
class QComboBox;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFBatesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFBatesDialog(pdf::PDFDocument* document, QWidget* parent = nullptr);
    ~PDFBatesDialog() override;

private slots:
    void onApplyClicked();

private:
    void initUi();

    QLineEdit* m_prefixEdit = nullptr;
    QSpinBox* m_startNumberSpin = nullptr;
    QSpinBox* m_digitsSpin = nullptr;
    QLineEdit* m_suffixEdit = nullptr;
    QComboBox* m_positionCombo = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFBATESDIALOG_H
