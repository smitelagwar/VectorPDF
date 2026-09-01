// MIT License
#include "reviewpackagestore.h"
#include "../package/workflowpackagewriter.h"
#include "../package/workflowpackagereader.h"

namespace vectorpdf::workflow
{

bool ReviewPackageStore::saveReview(const QString& packagePath, const ReviewDefinition& def)
{
    return WorkflowPackageWriter::writePackage(packagePath, QString(), def.metadata, def.reviewers);
}

bool ReviewPackageStore::loadReview(const QString& packagePath, ReviewDefinition& defOut)
{
    return WorkflowPackageReader::readPackage(packagePath, defOut.metadata, defOut.reviewers);
}

} // namespace vectorpdf::workflow
