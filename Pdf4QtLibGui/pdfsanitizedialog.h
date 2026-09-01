// MIT License
#ifndef PDFSANITIZEDIALOG_H
#define PDFSANITIZEDIALOG_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QCheckBox;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFSanitizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDFSanitizeDialog(pdf::PDFDocument* document, QWidget* parent = nullptr);
    ~PDFSanitizeDialog() override;

private slots:
    void onSanitizeClicked();

private:
    void initUi();

    QCheckBox* m_metaCheck = nullptr;
    QCheckBox* m_attachCheck = nullptr;
    QCheckBox* m_jsCheck = nullptr;
    QCheckBox* m_actionsCheck = nullptr;
    QCheckBox* m_layersCheck = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFSANITIZEDIALOG_H
