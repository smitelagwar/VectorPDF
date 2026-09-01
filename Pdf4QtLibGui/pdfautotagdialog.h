// MIT License
#ifndef PDFAUTOTAGDIALOG_H
#define PDFAUTOTAGDIALOG_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QComboBox;
class QLabel;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFAutoTagDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFAutoTagDialog(pdf::PDFDocument* document, QWidget* parent = nullptr);
    ~PDFAutoTagDialog() override;

private slots:
    void onAnalyzeClicked();
    void onApplyClicked();

private:
    void initUi();

    QComboBox* m_backendCombo = nullptr;
    QLabel* m_summaryLabel = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFAUTOTAGDIALOG_H
