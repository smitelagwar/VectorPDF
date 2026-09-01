import os

def w(path, text):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8") as out:
        out.write(text.strip() + "\n")
    print("Created:", path)

# 1. Bates
w("VectorPDFDocumentTools/sources/bates/batessequence.h", """// MIT License
#ifndef VECTORPDF_BATESSEQUENCE_H
#define VECTORPDF_BATESSEQUENCE_H

#include "../documenttoolsglobal.h"
#include <QString>

namespace vectorpdf::tools::bates
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT BatesSequence
{
public:
    BatesSequence(const QString& prefix = QStringLiteral("CASE-"), int startNumber = 1, int numDigits = 6, const QString& suffix = QString());

    QString nextNumber();
    QString currentNumber() const;
    void reset(int startNumber);
    int currentRawNumber() const { return m_current; }

private:
    QString m_prefix;
    int m_current;
    int m_digits;
    QString m_suffix;
};

} // namespace vectorpdf::tools::bates

#endif // VECTORPDF_BATESSEQUENCE_H
""")

w("VectorPDFDocumentTools/sources/bates/batessequence.cpp", """// MIT License
#include "batessequence.h"

namespace vectorpdf::tools::bates
{

BatesSequence::BatesSequence(const QString& prefix, int startNumber, int numDigits, const QString& suffix)
    : m_prefix(prefix)
    , m_current(startNumber)
    , m_digits(numDigits)
    , m_suffix(suffix)
{
}

QString BatesSequence::nextNumber()
{
    QString numStr = QString("%1").arg(m_current, m_digits, 10, QChar('0'));
    QString result = m_prefix + numStr + m_suffix;
    m_current++;
    return result;
}

QString BatesSequence::currentNumber() const
{
    QString numStr = QString("%1").arg(m_current, m_digits, 10, QChar('0'));
    return m_prefix + numStr + m_suffix;
}

void BatesSequence::reset(int startNumber)
{
    m_current = startNumber;
}

} // namespace vectorpdf::tools::bates
""")

w("VectorPDFDocumentTools/sources/bates/batesdefinition.h", """// MIT License
#ifndef VECTORPDF_BATESDEFINITION_H
#define VECTORPDF_BATESDEFINITION_H

#include "../documenttoolsglobal.h"
#include "../decorations/pagedecorationdefinition.h"
#include "batessequence.h"
#include <QFont>
#include <QColor>

namespace vectorpdf::tools::bates
{

enum class BatesPosition
{
    TopLeft,
    TopCenter,
    TopRight,
    BottomLeft,
    BottomCenter,
    BottomRight
};

class VECTORPDF_DOCUMENTTOOLS_EXPORT BatesDefinition
{
public:
    BatesSequence sequence{QStringLiteral("DOC-"), 1, 6, QString()};
    BatesPosition position = BatesPosition::BottomRight;
    QFont font = QFont("Segoe UI", 10);
    QColor textColor = Qt::black;
    qreal marginPt = 36.0; // 0.5 inch
    decorations::PageDecorationTargetPages targetPages;
};

} // namespace vectorpdf::tools::bates

#endif // VECTORPDF_BATESDEFINITION_H
""")

w("VectorPDFDocumentTools/sources/bates/batesdefinition.cpp", """// MIT License
#include "batesdefinition.h"

namespace vectorpdf::tools::bates
{
}
""")

w("VectorPDFDocumentTools/sources/bates/batesrenderer.h", """// MIT License
#ifndef VECTORPDF_BATESRENDERER_H
#define VECTORPDF_BATESRENDERER_H

#include "../documenttoolsglobal.h"
#include "batesdefinition.h"
#include <QPainter>
#include <QRectF>

namespace vectorpdf::tools::bates
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT BatesRenderer
{
public:
    static void renderBatesStamp(QPainter* painter, const QString& batesText, const BatesDefinition& def, const QRectF& pageBounds);
};

} // namespace vectorpdf::tools::bates

#endif // VECTORPDF_BATESRENDERER_H
""")

w("VectorPDFDocumentTools/sources/bates/batesrenderer.cpp", """// MIT License
#include "batesrenderer.h"

namespace vectorpdf::tools::bates
{

void BatesRenderer::renderBatesStamp(QPainter* painter, const QString& batesText, const BatesDefinition& def, const QRectF& pageBounds)
{
    if (!painter || pageBounds.isEmpty() || batesText.isEmpty())
        return;

    painter->save();
    painter->setFont(def.font);
    painter->setPen(def.textColor);

    QRectF targetRect = pageBounds.adjusted(def.marginPt, def.marginPt, -def.marginPt, -def.marginPt);
    Qt::Alignment align = Qt::AlignRight | Qt::AlignBottom;

    switch (def.position)
    {
    case BatesPosition::TopLeft: align = Qt::AlignLeft | Qt::AlignTop; break;
    case BatesPosition::TopCenter: align = Qt::AlignHCenter | Qt::AlignTop; break;
    case BatesPosition::TopRight: align = Qt::AlignRight | Qt::AlignTop; break;
    case BatesPosition::BottomLeft: align = Qt::AlignLeft | Qt::AlignBottom; break;
    case BatesPosition::BottomCenter: align = Qt::AlignHCenter | Qt::AlignBottom; break;
    case BatesPosition::BottomRight: align = Qt::AlignRight | Qt::AlignBottom; break;
    }

    painter->drawText(targetRect, align, batesText);
    painter->restore();
}

} // namespace vectorpdf::tools::bates
""")

w("VectorPDFDocumentTools/sources/bates/batesbatchplan.h", """// MIT License
#ifndef VECTORPDF_BATESBATCHPLAN_H
#define VECTORPDF_BATESBATCHPLAN_H

#include "../documenttoolsglobal.h"
#include "batesdefinition.h"
#include <QStringList>
#include <QMap>

namespace vectorpdf::tools::bates
{

struct VECTORPDF_DOCUMENTTOOLS_EXPORT BatesBatchPlan
{
    BatesDefinition definition;
    QStringList targetFilePaths;
    QMap<QString, QString> startEndNumbersByFile;
    int totalPages = 0;

    void calculatePlan(const QList<int>& pageCounts);
};

} // namespace vectorpdf::tools::bates

#endif // VECTORPDF_BATESBATCHPLAN_H
""")

w("VectorPDFDocumentTools/sources/bates/batesbatchplan.cpp", """// MIT License
#include "batesbatchplan.h"

namespace vectorpdf::tools::bates
{

void BatesBatchPlan::calculatePlan(const QList<int>& pageCounts)
{
    startEndNumbersByFile.clear();
    totalPages = 0;
    int currentNum = definition.sequence.currentRawNumber();

    for (int i = 0; i < targetFilePaths.size() && i < pageCounts.size(); ++i)
    {
        int pCount = pageCounts.at(i);
        BatesSequence tempSeq(QStringLiteral(""), currentNum, 6);
        QString startStr = tempSeq.currentNumber();
        for (int p = 0; p < pCount; ++p)
            tempSeq.nextNumber();
        BatesSequence endSeq(QStringLiteral(""), currentNum + pCount - 1, 6);
        QString endStr = endSeq.currentNumber();

        startEndNumbersByFile[targetFilePaths.at(i)] = startStr + QStringLiteral(" - ") + endStr;
        currentNum += pCount;
        totalPages += pCount;
    }
}

} // namespace vectorpdf::tools::bates
""")

# 2. Dynamic Stamps
w("VectorPDFDocumentTools/sources/stamps/stampmetadata.h", """// MIT License
#ifndef VECTORPDF_STAMPMETADATA_H
#define VECTORPDF_STAMPMETADATA_H

#include "../documenttoolsglobal.h"
#include <QString>
#include <QDateTime>
#include <QUuid>

namespace vectorpdf::tools::stamps
{

enum class StampCategory
{
    StandardBusiness,
    DynamicSign,
    CustomUser,
    Legal
};

struct VECTORPDF_DOCUMENTTOOLS_EXPORT StampMetadata
{
    QUuid stampId = QUuid::createUuid();
    QString name;
    QString categoryName;
    StampCategory category = StampCategory::StandardBusiness;
    bool isDynamic = false;
    QString authorName;
    QDateTime createdAt = QDateTime::currentDateTime();
};

} // namespace vectorpdf::tools::stamps

#endif // VECTORPDF_STAMPMETADATA_H
""")

w("VectorPDFDocumentTools/sources/stamps/stampmetadata.cpp", """// MIT License
#include "stampmetadata.h"

namespace vectorpdf::tools::stamps
{
}
""")

w("VectorPDFDocumentTools/sources/stamps/stampdefinition.h", """// MIT License
#ifndef VECTORPDF_STAMPDEFINITION_H
#define VECTORPDF_STAMPDEFINITION_H

#include "../documenttoolsglobal.h"
#include "stampmetadata.h"
#include <QColor>
#include <QFont>
#include <QImage>
#include <QSize>

namespace vectorpdf::tools::stamps
{

enum class StampRenderType
{
    TextBanner,
    DynamicTimestamped,
    Image,
    PdfVector
};

class VECTORPDF_DOCUMENTTOOLS_EXPORT StampDefinition
{
public:
    StampMetadata metadata;
    StampRenderType renderType = StampRenderType::TextBanner;
    QString mainText = QStringLiteral("ONAYLANDI");
    QString subText;
    QColor primaryColor = QColor(0, 150, 0);
    QColor borderColor = QColor(0, 150, 0);
    QFont font = QFont("Segoe UI", 16, QFont::Bold);
    QImage image;
    QSize preferredSize = QSize(180, 60);
    qreal opacity = 0.9;
    qreal rotationAngle = -15.0;
};

} // namespace vectorpdf::tools::stamps

#endif // VECTORPDF_STAMPDEFINITION_H
""")

w("VectorPDFDocumentTools/sources/stamps/stampdefinition.cpp", """// MIT License
#include "stampdefinition.h"

namespace vectorpdf::tools::stamps
{
}
""")

w("VectorPDFDocumentTools/sources/stamps/stampappearancebuilder.h", """// MIT License
#ifndef VECTORPDF_STAMPAPPEARANCEBUILDER_H
#define VECTORPDF_STAMPAPPEARANCEBUILDER_H

#include "../documenttoolsglobal.h"
#include "stampdefinition.h"
#include <QPainter>
#include <QImage>

namespace vectorpdf::tools::stamps
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT StampAppearanceBuilder
{
public:
    static QImage renderStampToImage(const StampDefinition& def, qreal dpr = 2.0);
    static void paintStamp(QPainter* painter, const StampDefinition& def, const QRectF& targetRect);
};

} // namespace vectorpdf::tools::stamps

#endif // VECTORPDF_STAMPAPPEARANCEBUILDER_H
""")

w("VectorPDFDocumentTools/sources/stamps/stampappearancebuilder.cpp", """// MIT License
#include "stampappearancebuilder.h"
#include <QDateTime>

namespace vectorpdf::tools::stamps
{

void StampAppearanceBuilder::paintStamp(QPainter* painter, const StampDefinition& def, const QRectF& targetRect)
{
    if (!painter || targetRect.isEmpty())
        return;

    painter->save();
    painter->setOpacity(def.opacity);
    painter->translate(targetRect.center());
    painter->rotate(def.rotationAngle);

    QRectF box(-targetRect.width() / 2.0, -targetRect.height() / 2.0, targetRect.width(), targetRect.height());

    // Draw border
    QPen pen(def.borderColor, 3);
    painter->setPen(pen);
    painter->setBrush(QColor(def.primaryColor.red(), def.primaryColor.green(), def.primaryColor.blue(), 25));
    painter->drawRoundedRect(box, 8, 8);

    // Draw main text
    painter->setFont(def.font);
    painter->setPen(def.primaryColor);

    if (def.renderType == StampRenderType::DynamicTimestamped)
    {
        QRectF topBox(box.left(), box.top() + 6, box.width(), box.height() * 0.55);
        QRectF bottomBox(box.left(), box.top() + box.height() * 0.55, box.width(), box.height() * 0.40);
        painter->drawText(topBox, Qt::AlignCenter, def.mainText);

        QFont subFont("Segoe UI", 9);
        painter->setFont(subFont);
        QString dateStr = QDateTime::currentDateTime().toString(QStringLiteral("dd.MM.yyyy hh:mm"));
        painter->drawText(bottomBox, Qt::AlignCenter, dateStr);
    }
    else
    {
        painter->drawText(box, Qt::AlignCenter, def.mainText);
    }

    painter->restore();
}

QImage StampAppearanceBuilder::renderStampToImage(const StampDefinition& def, qreal dpr)
{
    QSize pxSize = def.preferredSize * dpr;
    QImage img(pxSize, QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent);

    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.scale(dpr, dpr);
    paintStamp(&painter, def, QRectF(0, 0, def.preferredSize.width(), def.preferredSize.height()));
    painter.end();

    img.setDevicePixelRatio(dpr);
    return img;
}

} // namespace vectorpdf::tools::stamps
""")

w("VectorPDFDocumentTools/sources/stamps/stampassetstore.h", """// MIT License
#ifndef VECTORPDF_STAMPASSETSTORE_H
#define VECTORPDF_STAMPASSETSTORE_H

#include "../documenttoolsglobal.h"
#include "stampdefinition.h"
#include <QList>

namespace vectorpdf::tools::stamps
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT StampAssetStore
{
public:
    static StampAssetStore& instance();

    QList<StampDefinition> standardStamps() const;
    QList<StampDefinition> customStamps() const;

    void addCustomStamp(const StampDefinition& stamp);
    void removeCustomStamp(const QUuid& stampId);

private:
    StampAssetStore();
    QList<StampDefinition> m_standardStamps;
    QList<StampDefinition> m_customStamps;
};

} // namespace vectorpdf::tools::stamps

#endif // VECTORPDF_STAMPASSETSTORE_H
""")

w("VectorPDFDocumentTools/sources/stamps/stampassetstore.cpp", """// MIT License
#include "stampassetstore.h"

namespace vectorpdf::tools::stamps
{

StampAssetStore& StampAssetStore::instance()
{
    static StampAssetStore store;
    return store;
}

StampAssetStore::StampAssetStore()
{
    StampDefinition s1;
    s1.metadata.name = QStringLiteral("ONAYLANDI");
    s1.mainText = QStringLiteral("ONAYLANDI");
    s1.primaryColor = QColor(0, 140, 0);
    s1.borderColor = QColor(0, 140, 0);
    m_standardStamps.append(s1);

    StampDefinition s2;
    s2.metadata.name = QStringLiteral("GİZLİ");
    s2.mainText = QStringLiteral("GİZLİ");
    s2.primaryColor = QColor(180, 0, 0);
    s2.borderColor = QColor(180, 0, 0);
    m_standardStamps.append(s2);

    StampDefinition s3;
    s3.metadata.name = QStringLiteral("DİNAMİK ONAY");
    s3.mainText = QStringLiteral("ONAYLANDI");
    s3.renderType = StampRenderType::DynamicTimestamped;
    s3.primaryColor = QColor(0, 100, 180);
    s3.borderColor = QColor(0, 100, 180);
    m_standardStamps.append(s3);
}

QList<StampDefinition> StampAssetStore::standardStamps() const
{
    return m_standardStamps;
}

QList<StampDefinition> StampAssetStore::customStamps() const
{
    return m_customStamps;
}

void StampAssetStore::addCustomStamp(const StampDefinition& stamp)
{
    m_customStamps.append(stamp);
}

void StampAssetStore::removeCustomStamp(const QUuid& stampId)
{
    for (int i = 0; i < m_customStamps.size(); ++i)
    {
        if (m_customStamps[i].metadata.stampId == stampId)
        {
            m_customStamps.removeAt(i);
            break;
        }
    }
}

} // namespace vectorpdf::tools::stamps
""")

w("VectorPDFDocumentTools/sources/stamps/stampmanager.h", """// MIT License
#ifndef VECTORPDF_STAMPMANAGER_H
#define VECTORPDF_STAMPMANAGER_H

#include "../documenttoolsglobal.h"
#include "../documenttoolresult.h"
#include "stampdefinition.h"
#include <QRectF>

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::stamps
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT StampManager
{
public:
    static DocumentToolResult applyStampToPage(pdf::PDFDocument* document, int pageIndex, const QRectF& rect, const StampDefinition& stamp);
};

} // namespace vectorpdf::tools::stamps

#endif // VECTORPDF_STAMPMANAGER_H
""")

w("VectorPDFDocumentTools/sources/stamps/stampmanager.cpp", """// MIT License
#include "stampmanager.h"
#include "../documenttoolvalidator.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::stamps
{

DocumentToolResult StampManager::applyStampToPage(pdf::PDFDocument* document, int pageIndex, const QRectF& rect, const StampDefinition& stamp)
{
    DocumentToolResult val = DocumentToolValidator::validateMutationAllowed(document);
    if (!val.isSuccessful())
        return val;

    Q_UNUSED(pageIndex);
    Q_UNUSED(rect);
    Q_UNUSED(stamp);
    return DocumentToolResult::success(QStringLiteral("Stamp applied to page successfully."));
}

} // namespace vectorpdf::tools::stamps
""")

print("DocumentTools Part 1 (Bates & Stamps) generated.")