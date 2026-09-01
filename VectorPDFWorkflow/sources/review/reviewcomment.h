// MIT License
#ifndef VECTORPDF_REVIEWCOMMENT_H
#define VECTORPDF_REVIEWCOMMENT_H

#include "../workflowglobal.h"
#include "reviewreaction.h"
#include <QString>
#include <QDateTime>
#include <QRectF>
#include <QUuid>
#include <QList>

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT ReviewComment
{
    QUuid commentId = QUuid::createUuid();
    QUuid threadId;
    QUuid authorParticipantId;
    QString authorName;
    QString content;
    QDateTime createdAt = QDateTime::currentDateTime();
    int pageIndex = 0;
    QRectF targetRect;
    QList<ReviewReaction> reactions;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWCOMMENT_H
