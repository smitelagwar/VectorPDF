// MIT License
#ifndef VECTORPDF_FLATTENPOLICY_H
#define VECTORPDF_FLATTENPOLICY_H

#include "../documenttoolsglobal.h"

namespace vectorpdf::tools::flatten
{

struct VECTORPDF_DOCUMENTTOOLS_EXPORT FlattenPolicy
{
    bool forms = true;
    bool annotations = true;
    bool comments = true;
    bool stamps = true;
    bool drawings = true;
    bool freeText = true;
    bool layers = false;
    bool preserveLinks = true;
    bool preserveDigitalSignatures = true;
};

} // namespace vectorpdf::tools::flatten

#endif // VECTORPDF_FLATTENPOLICY_H
