// MIT License
#ifndef PDFAPPROVALSTATUSDOCK_H
#define PDFAPPROVALSTATUSDOCK_H

#include "pdf4qtlibgui_export.h"
#include <QDockWidget>

class QTreeWidget;
class QLabel;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFApprovalStatusDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit PDFApprovalStatusDock(QWidget* parent = nullptr);
    ~PDFApprovalStatusDock() override;

    void setDocument(pdf::PDFDocument* document);

private:
    void initUi();

    QTreeWidget* m_stepsTree = nullptr;
    QLabel* m_statusLabel = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFAPPROVALSTATUSDOCK_H
