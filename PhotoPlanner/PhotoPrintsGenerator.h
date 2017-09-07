#ifndef PHOTOPRINTSGENERATOR_H
#define PHOTOPRINTSGENERATOR_H

#include "PhotoPlannerCore.h"
#include "photogeocalcs.h"

#include <QPolygonF>
#include <QLineF>

#include "array2d.h"
#include "nearfilter.h"

namespace aero_photo {

//using LinedGeoPoints = QVector<std::tuple<qreal, GeoPoints>>;

class LinedGeoPoints : public QVector<GeoPoints> {
public:
    explicit LinedGeoPoints() { }
    explicit LinedGeoPoints(size_t size)
        : QVector<GeoPoints>(size)
        , azimuths_(size)
    {
    }

    void SetAzimuth(size_t index, qreal azimuth) { azimuths_[index] = azimuth; }
    qreal GetAzimuth(size_t index) const { return azimuths_[index]; }
private:
    void resize(int asize) = delete;
    QVector<qreal> azimuths_;
};

class PhotoPrintsGenerator {
public:
    virtual GeoPoints GeneratePhotoPrintsCenters() = 0;
};

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
        const int totalPrints = ceil(distance_/Lxp)+1;
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
    explicit LinearPhotoPrintsGenerator(const LinearPhotoRegion &linearRegion) {
        if (linearRegion.GetTrack().size()<2)
            throw std::logic_error("Invalid LinearPhotoRegion size()");
        trackHead_ = linearRegion.GetTrack().front();
        trackTail_ = linearRegion.GetTrack();
        trackTail_.pop_front();
    }

    LinedGeoPoints GeneratePhotoPrintsCenters(qreal Lxp, qreal Lyp, size_t totalRuns) {
        LinedGeoPoints linedGeoPoints(totalRuns * (trackTail_.size()));
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
                    runAzimuth += 180;
                    if (!pointRunPrintsCenters.empty())
                        for (int i = 0, j = pointRunPrintsCenters.size() - 1; i < j; ++i, --j)
                            std::swap(pointRunPrintsCenters[i], pointRunPrintsCenters[j]);
                }
                // We must calculate index
                const auto linedIndex = CalculateRunSequnce(trackIndex, runIndex);
                linedGeoPoints[linedIndex] = pointRunPrintsCenters;
                linedGeoPoints.SetAzimuth(linedIndex, runAzimuth);
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
    public:
        explicit RegionInternals(const AreaPhotoRegion &area) {
            if (area.GetBorder().size()<3)
                throw std::logic_error("Invalid AreaPhotoRegion size()");

            InitializeAreaPolygonAndBounds(area.GetBorder());
            InitializeGeoItems(area.GetBorder());
        }

        auto GetCenter() const { return geoCenter_; }
        auto GetRadius() const { return geoRadius_; }
        auto GetPreferredAzimuth() const { return preferredAzimuth_; }

        LinedGeoPoints GeneratePhotoPrintsCenters(qreal Lxp, qreal Lyp, qreal azimuth, size_t extentBorderValue) {
            auto geoPointsGrid = GeneratePhotoPrintsGrid(Lxp, Lyp, azimuth, extentBorderValue);
            return FilterPointsGrid(geoPointsGrid, extentBorderValue);
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
                    areaBounds_.contains(cartPoint) &&
                    areaPolygon_.containsPoint(cartPoint, Qt::FillRule::WindingFill) &&
                    true;
        }

        QPolygonF areaPolygon_;
        QRectF areaBounds_;

        GeoPoint geoCenter_;
        qreal geoRadius_ = 0;

        qreal preferredAzimuth_ = 0;

    private:
        class GeoPointsGrid {
        public:
            explicit GeoPointsGrid(size_t totalRuns) : linedGeoPoints_(totalRuns), isRevertedGeoPoint_(totalRuns, false) { }

            bool Empty() const { return linedGeoPoints_.empty() || linedGeoPoints_.front().empty(); }
            bool HasDifferentSizes() const {
                const auto rowSize = linedGeoPoints_.front().size();
                for (auto &rowGrid : linedGeoPoints_)
                    if (rowSize != rowGrid.size())
                        return true;
                return false;
            }

            void SetLine(size_t runIndex, GeoPoints &&geoPoints, bool isDirectOrder) {
                linedGeoPoints_[runIndex] = geoPoints;
                isRevertedGeoPoint_[runIndex] = !isDirectOrder;
            }

            size_t Rows() const { return linedGeoPoints_.size(); }
            size_t Cols() const { return linedGeoPoints_.front().size(); }
            auto GetGeoPoint(size_t rowIndex, size_t colIndex) const { return linedGeoPoints_[rowIndex][colIndex]; }

            void FixGeoPointsOrder(LinedGeoPoints &geoPoints) const {
                for (size_t iRow = 0; iRow < Rows(); ++iRow) {
                    if (isRevertedGeoPoint_[iRow]) {
                        std::vector<GeoPoint> revertedGeoPoints(geoPoints[iRow].crbegin(), geoPoints[iRow].crend());
                        geoPoints[iRow] = GeoPoints::fromStdVector(revertedGeoPoints);
                    }
                }
            }

        private:
            LinedGeoPoints linedGeoPoints_;
            std::vector<bool> isRevertedGeoPoint_;
        };

        GeoPointsGrid GeneratePhotoPrintsGrid(qreal Lxp, qreal Lyp, qreal azimuth, size_t extentBorderValue) {
            size_t totalRuns = ceil(GetRadius() * 2 / Lyp) + extentBorderValue;
            GeoPointsGrid geoPointsGrid(totalRuns);
            RunStartPointsCalc middlePointsCalc(GetCenter(), azimuth, Lyp, totalRuns);
            for (size_t i = 0; i < totalRuns; ++i) {
                auto middlePoint = middlePointsCalc.Calculate(i);
                const auto runAzimuth = azimuth ; // + 180 * ( i % 2 );
                auto endPntA = middlePoint.atDistanceAndAzimuth(GetRadius() + Lxp * extentBorderValue, runAzimuth + 180);
                auto endPntB = middlePoint.atDistanceAndAzimuth(GetRadius() + Lxp * extentBorderValue, runAzimuth);
                GeoCalc geoCalc;
                const auto runDistance = geoCalc.RoundUpPoints(endPntA, endPntB, Lxp);
                LinePhotoPrintsGenerator thisRunLineGenerator(endPntA, endPntA.azimuthTo(endPntB), runDistance);

                geoPointsGrid.SetLine(i, thisRunLineGenerator.GeneratePhotoPrintsCenters(Lxp), (i % 2) == 0);
            }
            return geoPointsGrid;
        }

        LinedGeoPoints FilterPointsGrid(const GeoPointsGrid &pointsGrid, size_t extentBorderValue) {

            if (pointsGrid.Empty())
                throw std::logic_error("GeoPoints grid is empty");
            if (pointsGrid.HasDifferentSizes())
                throw std::logic_error("GeoPoints grid has rows with different size");

            Array2D<uint8_t> containsArray(pointsGrid.Rows(), pointsGrid.Cols());
            auto initContainsItem = [this, &pointsGrid](auto iRow, auto iCol){
                return this->Contains(pointsGrid.GetGeoPoint(iRow, iCol));
            };
            containsArray.SetItems(initContainsItem);

            NearFilter<uint8_t> filter(extentBorderValue);
            auto filteredArray = filter(containsArray);

            LinedGeoPoints filteredGeoPoints(pointsGrid.Rows());
            auto fillFilteredGeoPoints = [&filteredGeoPoints, &pointsGrid](auto iRow, auto iCol, uint8_t itemValue) {
                if (itemValue)
                    filteredGeoPoints[iRow].push_back(pointsGrid.GetGeoPoint(iRow, iCol));
            };
            filteredArray.IterateItems(fillFilteredGeoPoints);

            pointsGrid.FixGeoPointsOrder(filteredGeoPoints);
            return filteredGeoPoints;
        }
    };

public:
    explicit AreaPhotoPrintsGenerator(const AreaPhotoRegion &area) : regionInternals_(area) {
    }

    LinedGeoPoints GeneratePhotoPrintsCenters(qreal Lxp, qreal Lyp, qreal azimuth, size_t extentBorderValue = 0) {
        return regionInternals_.GeneratePhotoPrintsCenters(Lxp, Lyp, azimuth, extentBorderValue);
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
    RegionInternals regionInternals_;
};


}

#endif // PHOTOPRINTSGENERATOR_H
