#ifndef PHOTOGEOUCALC_H
#define PHOTOGEOUCALC_H

#include "PhotoPlannerCore.h"
#include <math.h>
#include <cmath>
#include <QPointF>

namespace aero_photo {

constexpr qreal D2R(qreal degree) { return degree * M_PI / qreal(180); }
constexpr qreal R2D(qreal radian) { return radian / M_PI * qreal(180); }

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

class CartesianCalcs {
public:
    CartesianCalcs(GeoPoint basePoint, qreal baseAzimuth)
        : basePoint_(basePoint)
        , baseAzimuth_(baseAzimuth)
    {
    }

    QPointF ConvertToCartesian(const GeoPoint &point) {
        auto azimuthToPoint = D2R(basePoint_.azimuthTo(point));
        auto distanceToPoint = basePoint_.distanceTo(point);
        return QPointF(distanceToPoint * cos(azimuthToPoint), distanceToPoint * sin(azimuthToPoint));
    }

private:
    const GeoPoint basePoint_;
    const qreal baseAzimuth_;
};

class PhotoUavModel {
public:
    PhotoUavModel(qreal photoVelocity, qreal photoRoll) : velocity_(photoVelocity), roll_(photoRoll) { }

    qreal GetManeuverR() const { return velocity_ * velocity_ / 9.81 / tan(roll_); }

private:
    qreal velocity_;
    qreal roll_;
};

class ManeuverTrackAlignment {

    struct TurnPointData {
        GeoPoint center;
        bool isTurnRight = true;
    };

public:
    ManeuverTrackAlignment(const GeoPoint &pnt1, qreal az1, const GeoPoint &pnt2, qreal az2)
        : pnt1_(pnt1)
        , az1_(az1)
        , pnt2_(pnt2)
        , az2_(az2)
        , distance12_(pnt1.distanceTo(pnt2))
        , az12_(pnt1.azimuthTo(pnt2))
    {
    }

    GeoPoints Calculate(const PhotoUavModel &uavModel) {
        auto R = uavModel.GetManeuverR();

        auto getTurnPoint = [R](const GeoPoint &pnt, qreal az, qreal deltaAz) {
            if (deltaAz < -180)
                deltaAz += 360;
            bool isTurnRight = deltaAz >= 0;
            auto turnPoint = pnt.atDistanceAndAzimuth(R, az + (isTurnRight ? 90 : 270));
            return TurnPointData{turnPoint, isTurnRight};
        };

        auto turn1 = getTurnPoint(pnt1_, az1_, az12_ - az1_);
        auto turn2 = getTurnPoint(pnt2_, az2_, az2_ - az12_);

        if ( (turn1.isTurnRight && !turn2.isTurnRight) || (!turn1.isTurnRight && turn2.isTurnRight) )
            return CalculateTwoSideLine(turn1, turn2, R);
        else
            return CalculateOneSideLine(turn1, turn2, R);
    }

private:
    GeoPoints CalculateOneSideLine(auto &&turn1, auto &&turn2, qreal R) {
        auto az = turn1.center.azimuthTo(turn2.center) + (turn1.isTurnRight ? 270 : 90);
        auto t1mnv = turn1.center.atDistanceAndAzimuth(R, az);
        auto t2mnv = turn2.center.atDistanceAndAzimuth(R, az);
        //return GeoPoints{t1mnv, t2mnv};
        auto t12mnv = t1mnv.atDistanceAndAzimuth(t1mnv.distanceTo(t2mnv) / 2, t1mnv.azimuthTo(t2mnv));
//        return GeoPoints{t1mnv,
//                    //turn1.center,
//                    t12mnv,
//                    //turn2.center,
//                    t2mnv};
        GeoPoints points;
        AddCircle(points, turn1.center, R);
        points << t1mnv;
        points << t12mnv;
        points << t2mnv;
        AddCircle(points, turn2.center, R);
        return points;
    }
    GeoPoints CalculateTwoSideLine(auto &&turn1, auto &&turn2, qreal R) {
        auto halfDistance = turn1.center.distanceTo(turn2.center) / 2.0;
        auto deltaAz = asin(R/halfDistance);
        auto az = turn1.center.azimuthTo(turn2.center) + (turn1.isTurnRight ? -deltaAz : deltaAz);
        auto t1mnv = turn1.center.atDistanceAndAzimuth(R, az);
        auto t2mnv = turn2.center.atDistanceAndAzimuth(R, az + 180);
//        //return GeoPoints{t1mnv, t2mnv};
        auto t12mnv = t1mnv.atDistanceAndAzimuth(t1mnv.distanceTo(t2mnv) / 2, t1mnv.azimuthTo(t2mnv));
//        return GeoPoints{t1mnv,
//                    //turn1.center, t12mnv, turn2.center,
//                    t2mnv};
        GeoPoints points;
        AddCircle(points, turn1.center, R);
        points << t1mnv;
        points << t12mnv;
        points << t2mnv;
        AddCircle(points, turn2.center, R);
        return points;
    }

    void AddCircle(GeoPoints &points, const GeoPoint &center, qreal R) {
        for (qreal az = 0; az < 360; az += 30)
            points.push_back(center.atDistanceAndAzimuth(R, az));
        points.push_back(points.front());
    }

    const GeoPoint pnt1_;
    const qreal az1_;
    const GeoPoint pnt2_;
    const qreal az2_;

    const qreal distance12_;
    const qreal az12_;
};

}

#endif // PHOTOGEOUCALC_H
