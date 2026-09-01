// MIT License
#ifndef VECTORPDF_SHAREDFOLDERTRANSPORT_H
#define VECTORPDF_SHAREDFOLDERTRANSPORT_H

#include "filesystemtransport.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT SharedFolderTransport : public FileSystemTransport
{
public:
    QString transportId() const override { return QStringLiteral("transport.sharedfolder"); }
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_SHAREDFOLDERTRANSPORT_H
