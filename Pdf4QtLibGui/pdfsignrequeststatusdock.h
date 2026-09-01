// MIT License
#ifndef PDFSIGNREQUESTSTATUSDOCK_H
#define PDFSIGNREQUESTSTATUSDOCK_H

#include "pdf4qtlibgui_export.h"
#include <QDockWidget>

class QTreeWidget;
class QLabel;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFSignRequestStatusDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit PDFSignRequestStatusDock(QWidget* parent = nullptr);
    ~PDFSignRequestStatusDock() override;

    void setDocument(pdf::PDFDocument* document);

private:
    void initUi();

    QTreeWidget* m_statusTree = nullptr;
    QLabel* m_progressLabel = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFSIGNREQUESTSTATUSDOCK_H
