// MIT License
#ifndef PDFRECOVERYCENTER_H
#define PDFRECOVERYCENTER_H

#include "pdf4qtlibgui_export.h"
#include <QDialog>

class QListWidget;
class QLabel;

class PDF4QTLIBGUILIBSHARED_EXPORT PDFRecoveryCenter : public QDialog
{
    Q_OBJECT

public:
    explicit PDFRecoveryCenter(QWidget* parent = nullptr);
    ~PDFRecoveryCenter() override;

private slots:
    void onRestoreSelectedClicked();
    void onDiscardSelectedClicked();

private:
    void initUi();

    QListWidget* m_sessionsList = nullptr;
    QLabel* m_infoLabel = nullptr;
};

#endif // PDFRECOVERYCENTER_H
