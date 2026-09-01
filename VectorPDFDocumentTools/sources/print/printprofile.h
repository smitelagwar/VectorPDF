// MIT License
#ifndef VECTORPDF_PRINTPROFILE_H
#define VECTORPDF_PRINTPROFILE_H

#include "../documenttoolsglobal.h"

namespace vectorpdf::tools::print
{

enum class PrintLayoutMode
{
    Standard,
    NUpMultiplePages,
    Booklet,
    PosterTiling
};

enum class BookletBinding
{
    LeftBinding,
    RightBinding
};

struct VECTORPDF_DOCUMENTTOOLS_EXPORT PrintProfile
{
    PrintLayoutMode layoutMode = PrintLayoutMode::Standard;
    int nUpPagesPerSheet = 2; // 2, 4, 6, 9, 16
    BookletBinding bookletBinding = BookletBinding::LeftBinding;
    bool printColorSeparations = false;
    bool printCropMarks = false;
    bool printBleedMarks = false;
    bool autoRotateAndCenter = true;
    double scalePercentage = 100.0;
};

} // namespace vectorpdf::tools::print

#endif // VECTORPDF_PRINTPROFILE_H
