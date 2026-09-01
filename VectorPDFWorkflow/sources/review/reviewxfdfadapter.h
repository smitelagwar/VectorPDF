// MIT License
#ifndef VECTORPDF_REVIEWXFDFADAPTER_H
#define VECTORPDF_REVIEWXFDFADAPTER_H

#include "../workflowglobal.h"
#include "reviewdefinition.h"
#include <QString>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT ReviewXfdfAdapter
{
public:
    static QString exportThreadsToXfdf(const QList<ReviewThread>& threads);
    static QList<ReviewThread> importThreadsFromXfdf(const QString& xfdfContent);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWXFDFADAPTER_H
