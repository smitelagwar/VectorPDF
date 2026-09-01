// MIT License
#ifndef PDFSTAMPDOCK_H
#define PDFSTAMPDOCK_H

#include "pdf4qtlibgui_export.h"
#include <QDockWidget>

class QListWidget;
class QPushButton;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFStampDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit PDFStampDock(QWidget* parent = nullptr);
    ~PDFStampDock() override;

    void setDocument(pdf::PDFDocument* document);

private slots:
    void onAddCustomStampClicked();

private:
    void initUi();

    QListWidget* m_stampList = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFSTAMPDOCK_H
