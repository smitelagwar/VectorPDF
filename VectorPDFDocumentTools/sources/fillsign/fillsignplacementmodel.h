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

#ifndef VECTORPDF_FILLSIGNPLACEMENTMODEL_H
#define VECTORPDF_FILLSIGNPLACEMENTMODEL_H

#include "../documenttoolsglobal.h"
#include "signatureasset.h"
#include <QRectF>
#include <QUuid>
#include <QList>

namespace vectorpdf::tools::fillsign
{

struct VECTORPDF_DOCUMENTTOOLS_EXPORT VisualSignaturePlacement
{
    QUuid placementId;
    SignatureAsset asset;
    int pageIndex = 0;
    QRectF pageBoundingRect; // In PDF user space coordinates
    qreal rotationAngle = 0.0;
    qreal opacity = 1.0;
    bool isFlattened = false;
};

class VECTORPDF_DOCUMENTTOOLS_EXPORT FillSignPlacementModel
{
public:
    FillSignPlacementModel() = default;

    void addPlacement(const VisualSignaturePlacement& placement);
    bool removePlacement(const QUuid& placementId);
    void clearPlacements();
    QList<VisualSignaturePlacement> placementsForPage(int pageIndex) const;
    QList<VisualSignaturePlacement> allPlacements() const;

    void nudgePlacement(const QUuid& placementId, qreal dx, qreal dy);

private:
    QList<VisualSignaturePlacement> m_placements;
};

} // namespace vectorpdf::tools::fillsign

#endif // VECTORPDF_FILLSIGNPLACEMENTMODEL_H
