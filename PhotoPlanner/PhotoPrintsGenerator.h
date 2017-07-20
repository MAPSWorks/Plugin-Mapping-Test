#ifndef PHOTOPRINTSGENERATOR_H
#define PHOTOPRINTSGENERATOR_H

#include "PhotoPlannerCore.h"
#include "photogeocalcs.h"

#include <QPolygonF>

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
            pntA = pntB;
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


//class AreaPhotoPrintsGenerator {
//public:
//    AreaPhotoPrintsGenerator(const AreaPhotoRegion &area) {
//        if (area.GetBorder().size()<3)
//            throw std::logic_error("Invalid AreaPhotoRegion size()");

//        // azimuthOfPhotography_ = area.border_[0].azimuthTo(area.border_[1]);
//        areaRegion_.reserve(area.GetBorder().size());
//        for(auto geoPoint : area.GetBorder()) {
//          areaRegion_ << QPointF(geoPoint.latitude(), geoPoint.longitude());
//        }
//        boundingRect_ = areaRegion_.boundingRect();
//        boundingRadius_ = QLineF(boundingRect_.topLeft(), boundingRect_.bottomRight()).length()/2;
//        geoCenter_.setLatitude(boundingRect_.center().x());
//        geoCenter_.setLongitude(boundingRect_.center().y());
//    }

//    PhotoPrints GeneratePhotoPrints(qreal azimuth, qreal Lxp, qreal Lyp, qreal Lx, qreal Ly) {
//        PhotoPrints photoPrints;
//        size_t totalRuns = (boundingRadius_ / Lyp) + 1;
//        RunStartPointsCalc startPointsCalc(geoCenter_, azimuth, Lyp, totalRuns);
//        for (size_t i = 0; i < totalRuns; ++i) {
//            auto startPoint = startPointsCalc.Calculate(i);
//            auto endPointL = startPoint.atDistanceAndAzimuth(boundingRadius_, azimuth + 180);
//            auto endPointR = startPoint.atDistanceAndAzimuth(boundingRadius_, azimuth);
//            LinePhotoPrintsGenerator thisRunLineGenerator(endPointL, endPointR);
//            auto allPhotoPrintsCenters = thisRunLineGenerator.GeneratePhotoPrintsCenters(Lxp);
//            GeoPoints thisRunPhotoPrinCenters;
//            for(auto printCenter : allPhotoPrintsCenters) {
//                CartesianPoint cartPrintCenter(printCenter.latitude(), printCenter.longitude());
//                if (areaRegion_.containsPoint(cartPrintCenter, Qt::FillRule::OddEvenFill))
//                    thisRunPhotoPrinCenters.push_back(printCenter);
//            }
//            auto thisRunPhotoPrints = thisRunLineGenerator.GeneratePhotoPrints(thisRunPhotoPrinCenters, Lx, Ly);
//            photoPrints.append(thisRunPhotoPrints);
//        }
//        return photoPrints;
//    }

////    void GenerateLines(qreal azimuth, qreal Lx, qreal Ly) {
////        qreal azimuthUp = azimuth
////        for (qreal prodY = 0; true; ++prodY) {
////            qreal offset = Ly * prodY;
////        }
////    }

//private:
////    CalculateLine(const GeoPoint &geoPoint, qreal lineAzimuth) {
////        QLinkedList<GeoPoint> lineOfPhotoPrints;

////        auto processDistance = [this, &geoPoint, &lineOfPhotoPrints, ](qreal prod){
////            qreal distance = prod * Lx_;
////            auto distanceGeoPoint = geoPoint.atDistanceAndAzimuth(distance, azimuthOfPhotography_);
////            QPointF distancePoint(distancePoint.latitude(), distancePoint.longitude());
////            if (!boundingRect_.contains(distancePoint))
////                return false;
////            if (areaRegion_.containsPoint(distancePoint, Qt::FillRule::OddEvenFill))
////                lineOfPhotoPrints.push_back(distanceGeoPoint);
////            return true;
////        };

////        for(qreal prod = 0; processDistance(prod); ++prod) {
////        }

////        for(qreal prod = 0; processDistance(prod); ++prod) {
////        }
////    }

//    QPolygonF areaRegion_;
//    QRectF boundingRect_;
//    qreal boundingRadius_;
//    GeoPoint geoCenter_;

//    //qreal azimuthOfPhotography_;
//    //qreal Lx_, Ly_;
//};


}

#endif // PHOTOPRINTSGENERATOR_H
