// MIT License
#ifndef PDFADVANCEDPRINTDIALOG_H
#define PDFADVANCEDPRINTDIALOG_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QComboBox;
class QSpinBox;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFAdvancedPrintDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFAdvancedPrintDialog(pdf::PDFDocument* document, QWidget* parent = nullptr);
    ~PDFAdvancedPrintDialog() override;

private slots:
    void onPrintClicked();

private:
    void initUi();

    QComboBox* m_layoutCombo = nullptr;
    QComboBox* m_nUpCombo = nullptr;
    QSpinBox* m_copiesSpin = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFADVANCEDPRINTDIALOG_H
