// MIT License
//
// Copyright (c) 2026 VectorPDF Contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "signatureassetstore.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QDataStream>

#ifdef Q_OS_WIN
#include <windows.h>
#include <wincrypt.h>
#endif

namespace vectorpdf::tools::fillsign
{

SignatureAssetStore& SignatureAssetStore::instance()
{
    static SignatureAssetStore store;
    return store;
}

SignatureAssetStore::SignatureAssetStore()
{
    loadPersistedAssets();
}

void SignatureAssetStore::addAsset(const SignatureAsset& asset, bool persistOnDevice)
{
    if (persistOnDevice)
    {
        // Avoid duplicate ID
        m_persistedAssets.removeIf([&](const SignatureAsset& a) { return a.id == asset.id; });
        m_persistedAssets.append(asset);
        savePersistedAssets();
    }
    else
    {
        m_sessionAssets.removeIf([&](const SignatureAsset& a) { return a.id == asset.id; });
        m_sessionAssets.append(asset);
    }
}

QList<SignatureAsset> SignatureAssetStore::allAssets() const
{
    QList<SignatureAsset> res = m_persistedAssets;
    for (const auto& a : m_sessionAssets)
    {
        if (!res.contains(a))
        {
            res.append(a);
        }
    }
    return res;
}

QList<SignatureAsset> SignatureAssetStore::signatures() const
{
    QList<SignatureAsset> res;
    for (const auto& a : allAssets())
    {
        if (!a.isInitial)
            res.append(a);
    }
    return res;
}

QList<SignatureAsset> SignatureAssetStore::initials() const
{
    QList<SignatureAsset> res;
    for (const auto& a : allAssets())
    {
        if (a.isInitial)
            res.append(a);
    }
    return res;
}

bool SignatureAssetStore::removeAsset(const QUuid& id)
{
    int count1 = m_sessionAssets.removeIf([&](const SignatureAsset& a) { return a.id == id; });
    int count2 = m_persistedAssets.removeIf([&](const SignatureAsset& a) { return a.id == id; });
    if (count2 > 0)
    {
        savePersistedAssets();
    }
    return (count1 + count2) > 0;
}

void SignatureAssetStore::clearAll(bool wipePersistedStorage)
{
    m_sessionAssets.clear();
    if (wipePersistedStorage)
    {
        m_persistedAssets.clear();
        QFile::remove(storageFilePath());
    }
}

QString SignatureAssetStore::storageFilePath() const
{
    QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(appData);
    return appData + QStringLiteral("/signatures.dat");
}

QByteArray SignatureAssetStore::protectData(const QByteArray& plainData) const
{
#ifdef Q_OS_WIN
    if (plainData.isEmpty())
        return QByteArray();

    DATA_BLOB inBlob;
    inBlob.pbData = reinterpret_cast<BYTE*>(const_cast<char*>(plainData.data()));
    inBlob.cbData = static_cast<DWORD>(plainData.size());

    DATA_BLOB outBlob;
    if (CryptProtectData(&inBlob, L"VectorPDF Signature Data", nullptr, nullptr, nullptr, 0, &outBlob))
    {
        QByteArray protectedBytes(reinterpret_cast<char*>(outBlob.pbData), static_cast<int>(outBlob.cbData));
        LocalFree(outBlob.pbData);
        return protectedBytes;
    }
#endif
    // Fallback if not Windows or DPAPI not available
    return plainData;
}

QByteArray SignatureAssetStore::unprotectData(const QByteArray& encryptedData) const
{
#ifdef Q_OS_WIN
    if (encryptedData.isEmpty())
        return QByteArray();

    DATA_BLOB inBlob;
    inBlob.pbData = reinterpret_cast<BYTE*>(const_cast<char*>(encryptedData.data()));
    inBlob.cbData = static_cast<DWORD>(encryptedData.size());

    DATA_BLOB outBlob;
    if (CryptUnprotectData(&inBlob, nullptr, nullptr, nullptr, nullptr, 0, &outBlob))
    {
        QByteArray plainBytes(reinterpret_cast<char*>(outBlob.pbData), static_cast<int>(outBlob.cbData));
        LocalFree(outBlob.pbData);
        return plainBytes;
    }
#endif
    return encryptedData;
}

bool SignatureAssetStore::loadPersistedAssets()
{
    QString path = storageFilePath();
    QFile file(path);
    if (!file.exists() || !file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QByteArray encrypted = file.readAll();
    file.close();

    QByteArray plain = unprotectData(encrypted);
    QDataStream in(plain);
    in.setVersion(QDataStream::Qt_6_5);

    qint32 count;
    in >> count;
    m_persistedAssets.clear();

    for (qint32 i = 0; i < count; ++i)
    {
        QByteArray assetData;
        in >> assetData;
        m_persistedAssets.append(SignatureAsset::deserialize(assetData));
    }

    return true;
}

bool SignatureAssetStore::savePersistedAssets()
{
    QByteArray plain;
    QDataStream out(&plain, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);

    out << static_cast<qint32>(m_persistedAssets.size());
    for (const auto& a : m_persistedAssets)
    {
        out << a.serialize();
    }

    QByteArray encrypted = protectData(plain);
    QFile file(storageFilePath());
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(encrypted);
        file.close();
        return true;
    }
    return false;
}

} // namespace vectorpdf::tools::fillsign
