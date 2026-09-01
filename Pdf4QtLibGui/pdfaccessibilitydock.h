// MIT License
#ifndef PDFACCESSIBILITYDOCK_H
#define PDFACCESSIBILITYDOCK_H

#include "pdf4qtlibgui_export.h"
#include <QDockWidget>

class QTreeWidget;
class QPushButton;
class QLabel;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFAccessibilityDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit PDFAccessibilityDock(QWidget* parent = nullptr);
    ~PDFAccessibilityDock() override;

    void setDocument(pdf::PDFDocument* document);

private slots:
    void onRunCheckClicked();
    void onFixSelectedClicked();
    void onValidatePdfUaClicked();

private:
    void initUi();

    QTreeWidget* m_issuesTree = nullptr;
    QLabel* m_statusLabel = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFACCESSIBILITYDOCK_H
