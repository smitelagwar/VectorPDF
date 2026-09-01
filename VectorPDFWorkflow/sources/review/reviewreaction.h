// MIT License
#ifndef VECTORPDF_REVIEWREACTION_H
#define VECTORPDF_REVIEWREACTION_H

#include "../workflowglobal.h"
#include <QString>
#include <QUuid>

namespace vectorpdf::workflow
{

enum class ReviewReactionType
{
    ThumbsUp,
    ThumbsDown,
    Check,
    Question
};

struct VECTORPDF_WORKFLOW_EXPORT ReviewReaction
{
    QUuid reactionId = QUuid::createUuid();
    QUuid authorParticipantId;
    ReviewReactionType type = ReviewReactionType::ThumbsUp;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWREACTION_H
