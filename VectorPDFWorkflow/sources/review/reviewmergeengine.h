// MIT License
#ifndef VECTORPDF_REVIEWMERGEENGINE_H
#define VECTORPDF_REVIEWMERGEENGINE_H

#include "../workflowglobal.h"
#include "reviewdefinition.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT ReviewMergeEngine
{
public:
    static void mergeIncomingThreads(ReviewDefinition& target, const QList<ReviewThread>& incoming);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWMERGEENGINE_H
