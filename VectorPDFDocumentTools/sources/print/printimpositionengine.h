// MIT License
#ifndef VECTORPDF_PRINTIMPOSITIONENGINE_H
#define VECTORPDF_PRINTIMPOSITIONENGINE_H

#include "../documenttoolsglobal.h"
#include "printprofile.h"
#include <QList>
#include <QPair>

namespace vectorpdf::tools::print
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT PrintImpositionEngine
{
public:
    static QList<QList<int>> calculateNUpImposition(int totalPages, int nUpCount);
    static QList<QPair<int, int>> calculateBookletImposition(int totalPages);
};

} // namespace vectorpdf::tools::print

#endif // VECTORPDF_PRINTIMPOSITIONENGINE_H
