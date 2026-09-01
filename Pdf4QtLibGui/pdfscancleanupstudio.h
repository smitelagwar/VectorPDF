// MIT License
#ifndef PDFSCANCLEANUPSTUDIO_H
#define PDFSCANCLEANUPSTUDIO_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QCheckBox;
class QLabel;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFScanCleanupStudio : public QDialog
{
    Q_OBJECT

public:
    explicit PDFScanCleanupStudio(pdf::PDFDocument* document, QWidget* parent = nullptr);
    ~PDFScanCleanupStudio() override;

private slots:
    void onProcessClicked();

private:
    void initUi();

    QCheckBox* m_deskewCheck = nullptr;
    QCheckBox* m_rotateCheck = nullptr;
    QCheckBox* m_punchCheck = nullptr;
    QCheckBox* m_shadowCheck = nullptr;
    QCheckBox* m_bgNormCheck = nullptr;
    QLabel* m_previewLabel = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFSCANCLEANUPSTUDIO_H
