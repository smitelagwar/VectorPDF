// MIT License
#include "reviewservice.h"

namespace vectorpdf::workflow
{

ReviewState ReviewService::calculateState(const ReviewDefinition& def)
{
    ReviewState state;
    state.totalThreads = def.threads.size();
    for (const auto& t : def.threads)
    {
        if (t.status == ReviewThreadStatus::Open)
            state.openThreads++;
        else if (t.status == ReviewThreadStatus::Resolved)
            state.resolvedThreads++;
        state.totalComments += t.comments.size();
    }
    return state;
}

} // namespace vectorpdf::workflow
