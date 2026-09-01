// MIT License
#ifndef PDFHEADERFOOTERDIALOG_H
#define PDFHEADERFOOTERDIALOG_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QLineEdit;
class QComboBox;
class QSpinBox;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFHeaderFooterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFHeaderFooterDialog(pdf::PDFDocument* document, QWidget* parent = nullptr);
    ~PDFHeaderFooterDialog() override;

private slots:
    void onApplyClicked();

private:
    void initUi();

    QLineEdit* m_topLeftEdit = nullptr;
    QLineEdit* m_topCenterEdit = nullptr;
    QLineEdit* m_topRightEdit = nullptr;
    QLineEdit* m_bottomLeftEdit = nullptr;
    QLineEdit* m_bottomCenterEdit = nullptr;
    QLineEdit* m_bottomRightEdit = nullptr;

    QComboBox* m_pageRangeCombo = nullptr;
    QSpinBox* m_startPageSpin = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFHEADERFOOTERDIALOG_H
