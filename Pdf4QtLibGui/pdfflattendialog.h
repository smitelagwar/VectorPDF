// MIT License
#ifndef PDFFLATTENDIALOG_H
#define PDFFLATTENDIALOG_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QCheckBox;
class QLabel;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFFlattenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFFlattenDialog(pdf::PDFDocument* document, QWidget* parent = nullptr);
    ~PDFFlattenDialog() override;

private slots:
    void onAnalyzeClicked();
    void onFlattenClicked();

private:
    void initUi();

    QCheckBox* m_formsCheck = nullptr;
    QCheckBox* m_annotsCheck = nullptr;
    QCheckBox* m_stampsCheck = nullptr;
    QCheckBox* m_layersCheck = nullptr;
    QCheckBox* m_preserveLinksCheck = nullptr;
    QCheckBox* m_preserveSignaturesCheck = nullptr;
    QLabel* m_summaryLabel = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFFLATTENDIALOG_H
