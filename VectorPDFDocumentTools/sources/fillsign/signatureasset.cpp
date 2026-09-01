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

#include "signatureasset.h"
#include <QDataStream>
#include <QBuffer>

namespace vectorpdf::tools::fillsign
{

QByteArray SignatureAsset::serialize() const
{
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);

    out << id;
    out << static_cast<qint32>(type);
    out << displayName;
    out << isInitial;
    out << vectorPath;
    out << strokeWidth;
    out << inkColor;
    out << typedText;
    out << fontPresetName;
    out << image;
    out << rawImageData;

    return bytes;
}

SignatureAsset SignatureAsset::deserialize(const QByteArray& data)
{
    SignatureAsset asset;
    QDataStream in(data);
    in.setVersion(QDataStream::Qt_6_5);

    qint32 typeInt;
    in >> asset.id;
    in >> typeInt;
    asset.type = static_cast<SignatureAssetType>(typeInt);
    in >> asset.displayName;
    in >> asset.isInitial;
    in >> asset.vectorPath;
    in >> asset.strokeWidth;
    in >> asset.inkColor;
    in >> asset.typedText;
    in >> asset.fontPresetName;
    in >> asset.image;
    in >> asset.rawImageData;

    return asset;
}

} // namespace vectorpdf::tools::fillsign
