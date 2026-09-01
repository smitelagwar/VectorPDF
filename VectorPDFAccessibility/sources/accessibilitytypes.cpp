// MIT License
#include "accessibilitytypes.h"

namespace vectorpdf::a11y
{

QString tagTypeToString(StructureTagType type)
{
    switch (type)
    {
    case StructureTagType::Document: return QStringLiteral("Document");
    case StructureTagType::Part: return QStringLiteral("Part");
    case StructureTagType::Sect: return QStringLiteral("Sect");
    case StructureTagType::H1: return QStringLiteral("H1");
    case StructureTagType::H2: return QStringLiteral("H2");
    case StructureTagType::H3: return QStringLiteral("H3");
    case StructureTagType::H4: return QStringLiteral("H4");
    case StructureTagType::H5: return QStringLiteral("H5");
    case StructureTagType::H6: return QStringLiteral("H6");
    case StructureTagType::P: return QStringLiteral("P");
    case StructureTagType::L: return QStringLiteral("L");
    case StructureTagType::LI: return QStringLiteral("LI");
    case StructureTagType::Lbl: return QStringLiteral("Lbl");
    case StructureTagType::LBody: return QStringLiteral("LBody");
    case StructureTagType::Table: return QStringLiteral("Table");
    case StructureTagType::TR: return QStringLiteral("TR");
    case StructureTagType::TH: return QStringLiteral("TH");
    case StructureTagType::TD: return QStringLiteral("TD");
    case StructureTagType::Figure: return QStringLiteral("Figure");
    case StructureTagType::Caption: return QStringLiteral("Caption");
    case StructureTagType::Link: return QStringLiteral("Link");
    case StructureTagType::Form: return QStringLiteral("Form");
    case StructureTagType::Artifact: return QStringLiteral("Artifact");
    }
    return QStringLiteral("P");
}

StructureTagType stringToTagType(const QString& name)
{
    QString n = name.toUpper();
    if (n == QStringLiteral("DOCUMENT")) return StructureTagType::Document;
    if (n == QStringLiteral("PART")) return StructureTagType::Part;
    if (n == QStringLiteral("SECT")) return StructureTagType::Sect;
    if (n == QStringLiteral("H1")) return StructureTagType::H1;
    if (n == QStringLiteral("H2")) return StructureTagType::H2;
    if (n == QStringLiteral("H3")) return StructureTagType::H3;
    if (n == QStringLiteral("H4")) return StructureTagType::H4;
    if (n == QStringLiteral("H5")) return StructureTagType::H5;
    if (n == QStringLiteral("H6")) return StructureTagType::H6;
    if (n == QStringLiteral("P")) return StructureTagType::P;
    if (n == QStringLiteral("L")) return StructureTagType::L;
    if (n == QStringLiteral("LI")) return StructureTagType::LI;
    if (n == QStringLiteral("LBL")) return StructureTagType::Lbl;
    if (n == QStringLiteral("LBODY")) return StructureTagType::LBody;
    if (n == QStringLiteral("TABLE")) return StructureTagType::Table;
    if (n == QStringLiteral("TR")) return StructureTagType::TR;
    if (n == QStringLiteral("TH")) return StructureTagType::TH;
    if (n == QStringLiteral("TD")) return StructureTagType::TD;
    if (n == QStringLiteral("FIGURE")) return StructureTagType::Figure;
    if (n == QStringLiteral("CAPTION")) return StructureTagType::Caption;
    if (n == QStringLiteral("LINK")) return StructureTagType::Link;
    if (n == QStringLiteral("FORM")) return StructureTagType::Form;
    if (n == QStringLiteral("ARTIFACT")) return StructureTagType::Artifact;
    return StructureTagType::P;
}

} // namespace vectorpdf::a11y
