// MIT License
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
