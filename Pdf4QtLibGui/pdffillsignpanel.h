// MIT License
#ifndef PDFFILLSIGNPANEL_H
#define PDFFILLSIGNPANEL_H

#include "pdf4qtlibgui_export.h"
#include <QWidget>
#include <memory>

class QListWidget;
class QPushButton;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFFillSignPanel : public QWidget
{
    Q_OBJECT

public:
    explicit PDFFillSignPanel(QWidget* parent = nullptr);
    ~PDFFillSignPanel() override;

    void setDocument(pdf::PDFDocument* document);

signals:
    void signatureSelected(const QString& signatureId);
    void finalizeRequested(bool flatten);

private slots:
    void onAddSignatureClicked();
    void onAddInitialsClicked();
    void onClearAllSignatures();

private:
    void initUi();
    void reloadSignatures();

    QListWidget* m_signatureList = nullptr;
    QPushButton* m_addSignatureBtn = nullptr;
    QPushButton* m_addInitialsBtn = nullptr;
    QPushButton* m_clearBtn = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFFILLSIGNPANEL_H
