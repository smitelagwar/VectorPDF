// MIT License
#include "recoverycrypto.h"
#ifdef Q_OS_WIN
#include <windows.h>
#include <wincrypt.h>
#endif

namespace vectorpdf::recovery
{

QByteArray RecoveryCrypto::protectData(const QByteArray& plainData)
{
    if (plainData.isEmpty())
        return QByteArray();

#ifdef Q_OS_WIN
    DATA_BLOB inBlob;
    inBlob.pbData = reinterpret_cast<BYTE*>(const_cast<char*>(plainData.constData()));
    inBlob.cbData = static_cast<DWORD>(plainData.size());

    DATA_BLOB outBlob;
    if (CryptProtectData(&inBlob, L"VectorPDFRecoveryKey", nullptr, nullptr, nullptr, 0, &outBlob))
    {
        QByteArray result(reinterpret_cast<const char*>(outBlob.pbData), static_cast<int>(outBlob.cbData));
        LocalFree(outBlob.pbData);
        return result;
    }
#endif

    return plainData;
}

QByteArray RecoveryCrypto::unprotectData(const QByteArray& cipherData)
{
    if (cipherData.isEmpty())
        return QByteArray();

#ifdef Q_OS_WIN
    DATA_BLOB inBlob;
    inBlob.pbData = reinterpret_cast<BYTE*>(const_cast<char*>(cipherData.constData()));
    inBlob.cbData = static_cast<DWORD>(cipherData.size());

    DATA_BLOB outBlob;
    if (CryptUnprotectData(&inBlob, nullptr, nullptr, nullptr, nullptr, 0, &outBlob))
    {
        QByteArray result(reinterpret_cast<const char*>(outBlob.pbData), static_cast<int>(outBlob.cbData));
        LocalFree(outBlob.pbData);
        return result;
    }
#endif

    return cipherData;
}

} // namespace vectorpdf::recovery
