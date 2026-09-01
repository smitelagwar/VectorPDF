// MIT License
#ifndef VECTORPDF_SIGNREQUESTFIELD_H
#define VECTORPDF_SIGNREQUESTFIELD_H

#include "../workflowglobal.h"
#include <QString>
#include <QRectF>
#include <QUuid>

namespace vectorpdf::workflow
{

enum class SignFieldType
{
    Signature,
    Initials,
    Date,
    Text,
    Checkbox
};

struct VECTORPDF_WORKFLOW_EXPORT SignRequestField
{
    QUuid fieldId = QUuid::createUuid();
    QUuid assignedParticipantId;
    int pageIndex = 0;
    QRectF pageRect;
    SignFieldType type = SignFieldType::Signature;
    bool required = true;
    QString label;
    QString filledValue;
    bool isFilled = false;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_SIGNREQUESTFIELD_H
