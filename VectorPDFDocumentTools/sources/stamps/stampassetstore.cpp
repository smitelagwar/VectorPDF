// MIT License
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
