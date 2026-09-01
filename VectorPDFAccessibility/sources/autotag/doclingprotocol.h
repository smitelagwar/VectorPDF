// MIT License
#ifndef VECTORPDF_DOCLINGPROTOCOL_H
#define VECTORPDF_DOCLINGPROTOCOL_H

#include "../accessibilityglobal.h"
#include <QString>

namespace vectorpdf::a11y
{

struct VECTORPDF_ACCESSIBILITY_EXPORT DoclingProtocolVersion
{
    static constexpr int SchemaVersion = 1;
    static constexpr const char* ProtocolName = "VectorPDF.Docling.Bridge";
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_DOCLINGPROTOCOL_H
