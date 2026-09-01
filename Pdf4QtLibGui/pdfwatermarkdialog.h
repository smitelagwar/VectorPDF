// MIT License
#ifndef PDFWATERMARKDIALOG_H
#define PDFWATERMARKDIALOG_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QLineEdit;
class QSlider;
class QSpinBox;
class QComboBox;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFWatermarkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFWatermarkDialog(pdf::PDFDocument* document, QWidget* parent = nullptr);
    ~PDFWatermarkDialog() override;

private slots:
    void onApplyClicked();

private:
    void initUi();

    QLineEdit* m_textEdit = nullptr;
    QSlider* m_opacitySlider = nullptr;
    QSpinBox* m_rotationSpin = nullptr;
    QComboBox* m_layerCombo = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFWATERMARKDIALOG_H
