// MIT License
#ifndef PDFREVIEWDOCK_H
#define PDFREVIEWDOCK_H

#include "pdf4qtlibgui_export.h"
#include <QDockWidget>

class QTreeWidget;
class QPushButton;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFReviewDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit PDFReviewDock(QWidget* parent = nullptr);
    ~PDFReviewDock() override;

    void setDocument(pdf::PDFDocument* document);

private slots:
    void onSyncCommentsClicked();
    void onResolveSelectedClicked();

private:
    void initUi();

    QTreeWidget* m_commentsTree = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFREVIEWDOCK_H
