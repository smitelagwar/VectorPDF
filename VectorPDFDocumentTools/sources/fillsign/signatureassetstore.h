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

#ifndef VECTORPDF_SIGNATUREASSETSTORE_H
#define VECTORPDF_SIGNATUREASSETSTORE_H

#include "../documenttoolsglobal.h"
#include "signatureasset.h"
#include <QList>
#include <QString>

namespace vectorpdf::tools::fillsign
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT SignatureAssetStore
{
public:
    static SignatureAssetStore& instance();

    void addAsset(const SignatureAsset& asset, bool persistOnDevice = false);
    QList<SignatureAsset> allAssets() const;
    QList<SignatureAsset> signatures() const;
    QList<SignatureAsset> initials() const;
    bool removeAsset(const QUuid& id);
    void clearAll(bool wipePersistedStorage = true);

    bool loadPersistedAssets();
    bool savePersistedAssets();

private:
    SignatureAssetStore();
    ~SignatureAssetStore() = default;

    QByteArray protectData(const QByteArray& plainData) const;
    QByteArray unprotectData(const QByteArray& encryptedData) const;

    QString storageFilePath() const;

    QList<SignatureAsset> m_sessionAssets;
    QList<SignatureAsset> m_persistedAssets;
};

} // namespace vectorpdf::tools::fillsign

#endif // VECTORPDF_SIGNATUREASSETSTORE_H
