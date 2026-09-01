// MIT License
#ifndef VECTORPDF_STRUCTURETREEWRITER_H
#define VECTORPDF_STRUCTURETREEWRITER_H

#include "../accessibilityglobal.h"
#include "pdfstructuretreemodel.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::a11y
{

class VECTORPDF_ACCESSIBILITY_EXPORT StructureTreeWriter
{
public:
    static bool writeStructureTree(pdf::PDFDocument* document, const StructureNode* rootNode);
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_STRUCTURETREEWRITER_H
