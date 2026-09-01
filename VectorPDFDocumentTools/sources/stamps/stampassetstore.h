// MIT License
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
