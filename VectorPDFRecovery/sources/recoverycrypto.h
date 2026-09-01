// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_RECOVERYCRYPTO_H
#define VECTORPDF_RECOVERYCRYPTO_H

#include "recoveryglobal.h"
#include <QByteArray>

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoveryCrypto
{
public:
    static QByteArray protectData(const QByteArray& plainData);
    static QByteArray unprotectData(const QByteArray& cipherData);
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYCRYPTO_H
