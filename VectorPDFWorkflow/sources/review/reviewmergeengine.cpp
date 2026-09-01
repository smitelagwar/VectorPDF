// MIT License
#include "reviewmergeengine.h"

namespace vectorpdf::workflow
{

void ReviewMergeEngine::mergeIncomingThreads(ReviewDefinition& target, const QList<ReviewThread>& incoming)
{
    for (const auto& inThread : incoming)
    {
        bool found = false;
        for (auto& t : target.threads)
        {
            if (t.threadId == inThread.threadId)
            {
                t = inThread;
                found = true;
                break;
            }
        }
        if (!found)
        {
            target.threads.append(inThread);
        }
    }
}

} // namespace vectorpdf::workflow
