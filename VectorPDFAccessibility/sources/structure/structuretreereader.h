// MIT License
#ifndef VECTORPDF_STRUCTURETREEREADER_H
#define VECTORPDF_STRUCTURETREEREADER_H

#include "../accessibilityglobal.h"
#include "pdfstructuretreemodel.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::a11y
{

class VECTORPDF_ACCESSIBILITY_EXPORT StructureTreeReader
{
public:
    static bool parseStructureTree(const pdf::PDFDocument* document, StructureNode* targetRoot);
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_STRUCTURETREEREADER_H
