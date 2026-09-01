// MIT License
#ifndef PDFREADINGORDERDOCK_H
#define PDFREADINGORDERDOCK_H

#include "pdf4qtlibgui_export.h"
#include <QDockWidget>

class QListWidget;
class QPushButton;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFReadingOrderDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit PDFReadingOrderDock(QWidget* parent = nullptr);
    ~PDFReadingOrderDock() override;

    void setDocument(pdf::PDFDocument* document);

private slots:
    void onMoveUpClicked();
    void onMoveDownClicked();

private:
    void initUi();

    QListWidget* m_blockList = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFREADINGORDERDOCK_H
