// MIT License
#ifndef PDFFINDREPLACEDOCK_H
#define PDFFINDREPLACEDOCK_H

#include "pdf4qtlibgui_export.h"
#include <QDockWidget>

class QLineEdit;
class QCheckBox;
class QListWidget;
class QPushButton;

namespace pdf { class PDFDocument; }

class PDF4QTLIBGUILIBSHARED_EXPORT PDFFindReplaceDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit PDFFindReplaceDock(QWidget* parent = nullptr);
    ~PDFFindReplaceDock() override;

    void setDocument(pdf::PDFDocument* document);

private slots:
    void onSearchClicked();
    void onReplaceSelectedClicked();
    void onReplaceAllClicked();

private:
    void initUi();

    QLineEdit* m_findEdit = nullptr;
    QLineEdit* m_replaceEdit = nullptr;
    QCheckBox* m_caseCheck = nullptr;
    QCheckBox* m_wholeWordCheck = nullptr;
    QCheckBox* m_regexCheck = nullptr;
    QCheckBox* m_preserveCaseCheck = nullptr;
    QListWidget* m_resultsList = nullptr;
    pdf::PDFDocument* m_document = nullptr;
};

#endif // PDFFINDREPLACEDOCK_H
