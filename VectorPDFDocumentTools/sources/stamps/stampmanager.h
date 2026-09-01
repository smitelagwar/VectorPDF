// MIT License
#ifndef VECTORPDF_STAMPMANAGER_H
#define VECTORPDF_STAMPMANAGER_H

#include "../documenttoolsglobal.h"
#include "../documenttoolresult.h"
#include "stampdefinition.h"
#include <QRectF>

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::stamps
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT StampManager
{
public:
    static DocumentToolResult applyStampToPage(pdf::PDFDocument* document, int pageIndex, const QRectF& rect, const StampDefinition& stamp);
};

} // namespace vectorpdf::tools::stamps

#endif // VECTORPDF_STAMPMANAGER_H
