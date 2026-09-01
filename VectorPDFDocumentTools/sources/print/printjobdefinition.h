// MIT License
#ifndef VECTORPDF_PRINTJOBDEFINITION_H
#define VECTORPDF_PRINTJOBDEFINITION_H

#include "../documenttoolsglobal.h"
#include "printprofile.h"
#include <QString>

namespace vectorpdf::tools::print
{

struct VECTORPDF_DOCUMENTTOOLS_EXPORT PrintJobDefinition
{
    PrintProfile profile;
    QString printerName;
    int copies = 1;
    bool collate = true;
    QString pageRange; // e.g. "1-10"
};

} // namespace vectorpdf::tools::print

#endif // VECTORPDF_PRINTJOBDEFINITION_H
