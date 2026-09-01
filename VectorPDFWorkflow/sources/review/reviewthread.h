// MIT License
#ifndef VECTORPDF_REVIEWTHREAD_H
#define VECTORPDF_REVIEWTHREAD_H

#include "../workflowglobal.h"
#include "reviewcomment.h"
#include <QList>

namespace vectorpdf::workflow
{

enum class ReviewThreadStatus
{
    Open,
    Resolved,
    Accepted,
    Rejected
};

struct VECTORPDF_WORKFLOW_EXPORT ReviewThread
{
    QUuid threadId = QUuid::createUuid();
    int pageIndex = 0;
    QRectF anchorRect;
    ReviewThreadStatus status = ReviewThreadStatus::Open;
    QList<ReviewComment> comments;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWTHREAD_H
