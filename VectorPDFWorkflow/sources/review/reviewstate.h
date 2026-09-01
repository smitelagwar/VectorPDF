// MIT License
#ifndef VECTORPDF_REVIEWSTATE_H
#define VECTORPDF_REVIEWSTATE_H

#include "../workflowglobal.h"

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT ReviewState
{
    int totalThreads = 0;
    int openThreads = 0;
    int resolvedThreads = 0;
    int totalComments = 0;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWSTATE_H
