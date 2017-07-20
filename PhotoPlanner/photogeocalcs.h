#ifndef PHOTOGEOUCALC_H
#define PHOTOGEOUCALC_H

#include "PhotoPlannerCore.h"

namespace aero_photo {

using GeoDistance = qreal;

class GeoCalc {
public:
    qreal Azimuth(const GeoPoint &pntA, const GeoPoint &pntB) {
        return pntA.azimuthTo(pntB);
    }
    GeoDistance Distance(const GeoPoint &pntA, const GeoPoint &pntB) {
        return pntA.distanceTo(pntB);
    }
    GeoPoint AtDistanceAndAzimuth(const GeoPoint &pnt, GeoDistance distance, qreal azimuth) {
        return pnt.atDistanceAndAzimuth(distance, azimuth);
    }

    GeoDistance RoundUpPoints(GeoPoint &pntA, GeoPoint &pntB, GeoDistance factor) {
        const auto distance = Distance(pntA, pntB);
        auto deltaDistance = ceil(distance/factor)*factor - distance;

        if (deltaDistance != 0) {
            auto azimuthAB = Azimuth(pntA, pntB);
            auto azimuthBA = azimuthAB + 180;
            auto movedA = AtDistanceAndAzimuth(pntA, deltaDistance / 2, azimuthBA);
            auto movedB = AtDistanceAndAzimuth(movedA, distance + deltaDistance, azimuthAB);
            pntA = movedA;
            pntB = movedB;
        }
        return distance + deltaDistance;
    }
};

class RunStartPointsCalc {
public:
    RunStartPointsCalc(const GeoPoint &centerSidePoint, qreal azimuth, qreal Lyp, size_t totalRuns)
        : topLeftPoint_(CalculateTopLeftPoint(centerSidePoint, azimuth, Lyp, totalRuns))
        , startPointsAzimuth_(azimuth + 90)
        , Lyp_(Lyp)
        , totalRuns_(totalRuns) {
        //        assert(totalRuns_!=0);
    }
    RunStartPointsCalc(const GeoPoint &centerSidePoint, const GeoPoint &azimuthPoint, qreal Lyp, size_t totalRuns)
        : RunStartPointsCalc(centerSidePoint, centerSidePoint.azimuthTo(azimuthPoint), Lyp, totalRuns ) {
    }

    GeoPoint Calculate(size_t runIndex) {
        // Indexes from left to right in azimuth direction
        auto startPoint = topLeftPoint_;
        if(runIndex < totalRuns_) {
            startPoint = topLeftPoint_.atDistanceAndAzimuth(Lyp_ * runIndex, startPointsAzimuth_);
        }
        return startPoint;
    }

private:
    static GeoPoint CalculateTopLeftPoint(const GeoPoint &trackPoint, qreal azimuth, qreal Lyp, size_t totalRuns) {
        if (totalRuns < 2)
            return trackPoint;
        auto baseSize = Lyp * (totalRuns - 1);
        return trackPoint.atDistanceAndAzimuth(baseSize / 2, azimuth - 90);
    }

    const GeoPoint topLeftPoint_;
    const qreal startPointsAzimuth_;
    const qreal Lyp_;
    const size_t totalRuns_;
};

}

#endif // PHOTOGEOUCALC_H
