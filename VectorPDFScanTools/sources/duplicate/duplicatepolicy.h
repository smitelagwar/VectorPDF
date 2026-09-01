// MIT License
#ifndef VECTORPDF_DUPLICATEPOLICY_H
#define VECTORPDF_DUPLICATEPOLICY_H

#include "../scantoolsglobal.h"

namespace vectorpdf::scan
{

struct VECTORPDF_SCANTOOLS_EXPORT DuplicatePolicy
{
    double minVisualSimilarity = 0.92; // 92% visual similarity
    bool requireExactTextMatch = false;
    bool compareAcrossAllPages = true;
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_DUPLICATEPOLICY_H
