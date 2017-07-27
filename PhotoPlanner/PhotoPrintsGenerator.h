#ifndef PHOTOPRINTSGENERATOR_H
#define PHOTOPRINTSGENERATOR_H

#include "PhotoPlannerCore.h"
#include "photogeocalcs.h"

#include <QPolygonF>
#include <QLineF>

namespace aero_photo {

using LinedGeoPoints = QVector<GeoPoints>;

class LinePhotoPrintsGenerator {
public:
    LinePhotoPrintsGenerator(const GeoPoint &pntA, qreal azimuth, GeoDistance distance)
        : pntA_(pntA)
        , azimuth_(azimuth)
        , distance_(distance) {
    }

    LinePhotoPrintsGenerator(const GeoPoint &pntA, const GeoPoint &pntB)
        : LinePhotoPrintsGenerator(pntA, pntA.azimuthTo(pntB), pntA.distanceTo(pntB)) {
    }

    GeoPoints GeneratePhotoPrintsCenters(qreal Lxp) {
        GeoPoints photoPrintsCenters;
        const int totalPrints = distance_/Lxp + 1;
        for (int i = 0; i<totalPrints; ++i) {
            auto nextCenter = pntA_.atDistanceAndAzimuth(Lxp*i, azimuth_);
            photoPrintsCenters.push_back(nextCenter);
        }
        return photoPrintsCenters;
    }
    PhotoPrints GeneratePhotoPrints(const GeoPoints &photoPrintsCenters, qreal Lx, qreal Ly) {
        PhotoPrints linePhotoPrints;
        auto photoPrintHalfDiag = sqrt(Lx*Lx+Ly*Ly)/2.0;
        auto generatePhotoPrintBorder = [this,photoPrintHalfDiag](const GeoPoint &photoPrintCenter){
            auto ppLT = photoPrintCenter.atDistanceAndAzimuth(photoPrintHalfDiag, azimuth_ - 45);
            auto ppRT = photoPrintCenter.atDistanceAndAzimuth(photoPrintHalfDiag, azimuth_ + 45);
            auto ppLB = photoPrintCenter.atDistanceAndAzimuth(photoPrintHalfDiag, azimuth_ - 135);
            auto ppRB = photoPrintCenter.atDistanceAndAzimuth(photoPrintHalfDiag, azimuth_ + 135);
            GeoPoints photoPrintBorder = {ppLT, ppRT, ppLB, ppRB};
            return photoPrintBorder;
        };
        for (auto &&photoPrintCenter : photoPrintsCenters) {
            linePhotoPrints << PhotoPrint(photoPrintCenter, generatePhotoPrintBorder(photoPrintCenter));
        }
        return linePhotoPrints;
    }
    PhotoPrints GeneratePhotoPrints(qreal Lxp, qreal Lx, qreal Ly) {
        auto photoPrintsCenters = GeneratePhotoPrintsCenters(Lxp);
        return GeneratePhotoPrints(photoPrintsCenters, Lx, Ly);
    }

private:
    const GeoPoint pntA_;
    const qreal azimuth_;
    const qreal distance_;
};

class LinearPhotoPrintsGenerator {
public:
    LinearPhotoPrintsGenerator(const LinearPhotoRegion &linearRegion) {
        if (linearRegion.GetTrack().size()<2)
            throw std::logic_error("Invalid LinearPhotoRegion size()");
        trackHead_ = linearRegion.GetTrack().front();
        trackTail_ = linearRegion.GetTrack();
        trackTail_.pop_front();
    }

    LinedGeoPoints GeneratePhotoPrintsCenters(qreal Lxp, qreal Lyp, size_t totalRuns) {
        LinedGeoPoints linedGeoPoints;
        linedGeoPoints.resize(totalRuns * (trackTail_.size()));
        // !!! Both pntA & pntB must be copy of members
        auto pntA = trackHead_;
        for (int trackIndex = 0; trackIndex < trackTail_.size(); trackIndex++) {
            auto pntB = trackTail_[trackIndex];
            GeoCalc geoCalc;
            const auto distance = geoCalc.RoundUpPoints(pntA, pntB, Lxp);
            RunStartPointsCalc pointsA(pntA, pntB, Lyp, totalRuns);
            RunStartPointsCalc pointsB(pntB, geoCalc.Azimuth(pntA, pntB), Lyp, totalRuns);
            for(size_t runIndex = 0; runIndex < totalRuns; runIndex++) {
                auto startPointAB = pointsA.Calculate(runIndex);
                auto finishPointAB = pointsB.Calculate(runIndex);
                auto runAzimuth = geoCalc.Azimuth(startPointAB, finishPointAB);
                LinePhotoPrintsGenerator thisRunLineGenerator(startPointAB, runAzimuth, distance);
                auto pointRunPrintsCenters = thisRunLineGenerator.GeneratePhotoPrintsCenters(Lxp);
                if (runIndex % 2 != 0) {
                    if (!pointRunPrintsCenters.empty())
                        for (int i = 0, j = pointRunPrintsCenters.size() - 1; i < j; ++i, --j)
                            std::swap(pointRunPrintsCenters[i], pointRunPrintsCenters[j]);
                }
                // We must calculate index
                linedGeoPoints[CalculateRunSequnce(trackIndex, runIndex)] = pointRunPrintsCenters;
            }
            pntA = trackTail_[trackIndex];
        }
        return linedGeoPoints;
    }

    PhotoPrints GeneratePhotoPrints(const LinedGeoPoints &linedGeoPoints, qreal Lx, qreal Ly) {
        PhotoPrints photoPrints;
        //photoPrints.reserve(linedGeoPoints.size());
        for (auto &&linePoints : linedGeoPoints) {
            LinePhotoPrintsGenerator thisRunLineGenerator(linePoints.front(), linePoints.back());
            auto thisRunLinePhotoPrints = thisRunLineGenerator.GeneratePhotoPrints(linePoints, Lx, Ly);
            photoPrints.append(thisRunLinePhotoPrints);
        }
        photoPrints.squeeze();
        return photoPrints;
    }

    int CalculateRunSequnce(int trackIndex, int runIndex) {
        const auto totalTracks = trackTail_.size();
        auto sequence =
                ((runIndex + 1) % 2) * trackIndex +
                (runIndex % 2) * (totalTracks - 1 - trackIndex) +
                runIndex * totalTracks;
        return sequence;
    }

private:
    GeoPoint trackHead_;
    GeoPoints trackTail_;
};


class AreaPhotoPrintsGenerator {

    class RegionInternals {
    private:
        auto InitializeFilteredMarks(const LinedGeoPoints &pointsGrid) {
            const auto filteredRowSize = pointsGrid.front().size() + 2;
            for (auto &rowGrid : pointsGrid)
                if (filteredRowSize != rowGrid.size() + 2)
                    throw std::logic_error("GeoPoints grid has rows with different size");

            using FilteredMarks = QVector<QVector<bool>>;
            FilteredMarks filteredMarks(pointsGrid.size() + 2);
            for (auto &rowMarks : filteredMarks)
                rowMarks.resize(filteredRowSize);

            for (int iRow = 1; iRow < filteredMarks.size() - 1; ++iRow)
                for (int iColumn = 1; iColumn < filteredMarks[iRow].size() - 1; ++iColumn) {
                    filteredMarks[iRow][iColumn] = Contains(pointsGrid[iRow-1][iColumn-1]);
                }
            return filteredMarks;
        }
        LinedGeoPoints CreateFilteredGeoPoints(auto &&filteredMarks, const LinedGeoPoints &pointsGrid) {
            LinedGeoPoints filteredGeoPoints(pointsGrid.size());
            for (int iRow = 1; iRow < filteredMarks.size() - 1; ++iRow)
                for (int iColumn = 1; iColumn < filteredMarks[iRow].size() - 1; ++iColumn) {
                    if (   // self
                            filteredMarks[iRow][iColumn] ||
                            // horizontal & vertical
//                           filteredMarks[iRow - 1][iColumn] ||
//                           filteredMarks[iRow][iColumn - 1] ||
//                           filteredMarks[iRow][iColumn + 1] ||
//                           filteredMarks[iRow + 1][iColumn] ||
                           // diagonal
//                           filteredMarks[iRow - 1][iColumn - 1] ||
//                           filteredMarks[iRow - 1][iColumn + 1] ||
//                           filteredMarks[iRow + 1][iColumn - 1] ||
//                           filteredMarks[iRow + 1][iColumn + 1] ||
                           //
                           false
                           )
                        filteredGeoPoints[iRow-1].push_back(pointsGrid[iRow-1][iColumn-1]);
                }
            return filteredGeoPoints;
        }

    public:
        RegionInternals(const AreaPhotoRegion &area) {
            if (area.GetBorder().size()<3)
                throw std::logic_error("Invalid AreaPhotoRegion size()");

            InitializeAreaPolygonAndBounds(area.GetBorder());
            InitializeGeoItems(area.GetBorder());
        }

        auto GetCenter() const { return geoCenter_; }
        auto GetRadius() const { return geoRadius_; }
        auto GetPreferredAzimuth() const { return preferredAzimuth_; }

        auto FilterPointsGrid(const LinedGeoPoints &pointsGrid) {
            LinedGeoPoints outPoints;
            if (!pointsGrid.empty() && !pointsGrid.front().empty()) {
                auto filteredMarks = InitializeFilteredMarks(pointsGrid);
                outPoints = CreateFilteredGeoPoints(filteredMarks, pointsGrid);
            }
            return outPoints;
        }

    private:
        void InitializeAreaPolygonAndBounds(const GeoPoints &border) {
            areaPolygon_.reserve(border.size());
            for(auto geoPoint : border) {
              areaPolygon_ << QPointF(geoPoint.latitude(), geoPoint.longitude());
            }
            if (!areaPolygon_.isClosed())
                areaPolygon_ << areaPolygon_.front();
            areaBounds_ = areaPolygon_.boundingRect();
        }
        void InitializeGeoItems(const GeoPoints &border) {
            geoCenter_.setLatitude(areaBounds_.center().x());
            geoCenter_.setLongitude(areaBounds_.center().y());

            for(auto borderPoint : border) {
                auto distanceToBorder = geoCenter_.distanceTo(borderPoint);
                if (geoRadius_ < distanceToBorder) {
                    geoRadius_ = distanceToBorder;
                    preferredAzimuth_ = geoCenter_.azimuthTo(borderPoint);
                }
            }
        }


        bool Contains(const GeoPoint &geoPoint) const {
            CartesianPoint cartPoint(geoPoint.latitude(), geoPoint.longitude());
            return
                    //areaBounds_.contains(cartPoint) &&
                    areaPolygon_.containsPoint(cartPoint, Qt::FillRule::WindingFill) &&
                    true;
        }

        QPolygonF areaPolygon_;
        QRectF areaBounds_;

        GeoPoint geoCenter_;
        qreal geoRadius_ = 0;

        qreal preferredAzimuth_ = 0;
    };

public:
    AreaPhotoPrintsGenerator(const AreaPhotoRegion &area) : regionInternals_(area) {
    }

    LinedGeoPoints GeneratePhotoPrintsCenters(qreal Lxp, qreal Lyp, qreal azimuth) {
        auto geoPointsGrid = GeneratePhotoPrintsGrid(Lxp, Lyp, azimuth);
        auto linedGeoPoints = regionInternals_.FilterPointsGrid(geoPointsGrid);
        return linedGeoPoints;
    }

    PhotoPrints GeneratePhotoPrints(const LinedGeoPoints &linedGeoPoints, qreal Lx, qreal Ly) {
        PhotoPrints photoPrints;
        for (auto &&linePoints : linedGeoPoints) {
            if (!linePoints.empty()) {
                LinePhotoPrintsGenerator thisRunLineGenerator(linePoints.front(), linePoints.back());
                auto thisRunLinePhotoPrints = thisRunLineGenerator.GeneratePhotoPrints(linePoints, Lx, Ly);
                photoPrints.append(thisRunLinePhotoPrints);
            }
        }
        photoPrints.squeeze();
        return photoPrints;
    }

    auto GetPreferredAzimuth() const { return regionInternals_.GetPreferredAzimuth(); }

private:

    LinedGeoPoints GeneratePhotoPrintsGrid(qreal Lxp, qreal Lyp, qreal azimuth) {
        LinedGeoPoints geoPointsGrid;
        size_t totalRuns = ceil(regionInternals_.GetRadius() * 2 / Lyp);
        geoPointsGrid.resize(totalRuns);
        RunStartPointsCalc middlePointsCalc(regionInternals_.GetCenter(), azimuth, Lyp, totalRuns);
        for (size_t i = 0; i < totalRuns; ++i) {
            auto middlePoint = middlePointsCalc.Calculate(i);
            const auto runAzimuth = azimuth + 180 * ( i % 2 );
            auto endPntA = middlePoint.atDistanceAndAzimuth(regionInternals_.GetRadius(), runAzimuth + 180);
            auto endPntB = middlePoint.atDistanceAndAzimuth(regionInternals_.GetRadius(), runAzimuth);
            GeoCalc geoCalc;
            const auto runDistance = geoCalc.RoundUpPoints(endPntA, endPntB, Lxp);
            LinePhotoPrintsGenerator thisRunLineGenerator(endPntA, endPntA.azimuthTo(endPntB), runDistance);
            geoPointsGrid[i] = thisRunLineGenerator.GeneratePhotoPrintsCenters(Lxp);
        }
        return geoPointsGrid;
    }


    RegionInternals regionInternals_;
};


}

#endif // PHOTOPRINTSGENERATOR_H
