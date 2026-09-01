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

#include "fillsignplacementmodel.h"

namespace vectorpdf::tools::fillsign
{

void FillSignPlacementModel::addPlacement(const VisualSignaturePlacement& placement)
{
    m_placements.append(placement);
}

bool FillSignPlacementModel::removePlacement(const QUuid& placementId)
{
    return m_placements.removeIf([&](const VisualSignaturePlacement& p) { return p.placementId == placementId; }) > 0;
}

void FillSignPlacementModel::clearPlacements()
{
    m_placements.clear();
}

QList<VisualSignaturePlacement> FillSignPlacementModel::placementsForPage(int pageIndex) const
{
    QList<VisualSignaturePlacement> list;
    for (const auto& p : m_placements)
    {
        if (p.pageIndex == pageIndex)
            list.append(p);
    }
    return list;
}

QList<VisualSignaturePlacement> FillSignPlacementModel::allPlacements() const
{
    return m_placements;
}

void FillSignPlacementModel::nudgePlacement(const QUuid& placementId, qreal dx, qreal dy)
{
    for (auto& p : m_placements)
    {
        if (p.placementId == placementId)
        {
            p.pageBoundingRect.translate(dx, dy);
            break;
        }
    }
}

} // namespace vectorpdf::tools::fillsign
