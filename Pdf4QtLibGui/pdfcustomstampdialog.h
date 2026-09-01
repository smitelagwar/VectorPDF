// MIT License
#ifndef PDFCUSTOMSTAMPDIALOG_H
#define PDFCUSTOMSTAMPDIALOG_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QLineEdit;
class QComboBox;

class PDF4QTLIBGUILIBSHARED_EXPORT PDFCustomStampDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFCustomStampDialog(QWidget* parent = nullptr);
    ~PDFCustomStampDialog() override;

private:
    void initUi();

    QLineEdit* m_textEdit = nullptr;
    QComboBox* m_colorCombo = nullptr;
};

#endif // PDFCUSTOMSTAMPDIALOG_H
