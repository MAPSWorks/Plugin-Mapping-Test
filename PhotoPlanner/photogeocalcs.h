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


template <typename TDegreeType>
struct DegreeTraits {
    static constexpr TDegreeType Zero = {};
    static constexpr TDegreeType MaxUnique = {360};
};

// TAzimuth area of value [-180; 180]
template <typename TAzimuthType = qreal>
class TAzimuth {
public:
    using ValueType = TAzimuthType;
    using AzimuthTraits = DegreeTraits<ValueType>;

    TAzimuth() {}
    TAzimuth(TAzimuthType degree) : degree_(NormalizeValue(degree)) {
    }
    TAzimuth(const TAzimuth &other) : degree_(other.degree_) {
    }
    TAzimuth(TAzimuth &&other) : degree_(other.degree_) {
        other.degree_ = AzimuthTraits::Zero;
    }

    TAzimuth &operator= (const TAzimuth &other) {
        degree_ = other.degree_;
        return *this;
    }
    TAzimuth &operator= (TAzimuthType degree) {
        degree_ = NormalizeValue(degree);
        return *this;
    }

    TAzimuth &operator+= (const TAzimuth &other) {
        degree_ = NormalizeValue(degree_ + other.degree_);
        return *this;
    }
    TAzimuth &operator-= (const TAzimuth &other) {
        degree_ = NormalizeValue(degree_ - other.degree_);
        return *this;
    }

    operator ValueType() const { return degree_; }

private:

    inline static ValueType NormalizeValue(ValueType degree) {
        degree = std::remainder(degree, AzimuthTraits::MaxUnique);
        if (degree == 180)
            return -180;
        return degree;
    }
    ValueType degree_ = AzimuthTraits::Zero;
};

template <typename TAzimuthType>
TAzimuth<TAzimuthType> &operator + (const TAzimuth<TAzimuthType> &lft, const TAzimuth<TAzimuthType> &rht) {
    TAzimuth<TAzimuthType> ret(lft);
    return ret+=rht;
}

template <typename TAzimuthType>
TAzimuth<TAzimuthType> &operator - (const TAzimuth<TAzimuthType> &lft, const TAzimuth<TAzimuthType> &rht) {
    TAzimuth<TAzimuthType> ret(lft);
    return ret-=rht;
}

using Azimuth =  TAzimuth<qreal>;

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
            pntA = AtDistanceAndAzimuth(pntA, deltaDistance / 2, azimuthBA);
            pntB = AtDistanceAndAzimuth(pntA, distance + deltaDistance, azimuthAB);
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
    PhotoUavModel(qreal photoVelocityMPerSec, qreal photoRollRadian) : velocity_(photoVelocityMPerSec), roll_(photoRollRadian) { }

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

        if (2*R < distance12_ )
            return CalculateMoreThen2RAligment(R);

        //if ( distance12_ < R)
        {
        }

        ManeuverTrackAlignment enlargedEntry(pnt1_, az1_, pnt2_.atDistanceAndAzimuth(R, az2_ + Azimuth(180)), az2_);
        auto geoPoints = enlargedEntry.Calculate(uavModel);
        geoPoints.push_back(pnt2_);
        return geoPoints;
    }

private:
    GeoPoints CalculateMoreThen2RAligment(qreal R) {
        auto t1R = GetTurnPointData(R, pnt1_, az1_, true);
        auto t1L = GetTurnPointData(R, pnt1_, az1_, false);
        auto t2R = GetTurnPointData(R, pnt2_, az2_, true);
        auto t2L = GetTurnPointData(R, pnt2_, az2_, false);

        std::map<qreal, GeoPoints> maneuverLengths;
        auto addTurnPairsChecked = [this, &maneuverLengths, R](auto &&t1, auto &&t2) {
            auto distance = t1.center.distanceTo(t2.center);
            if ((t1.isTurnRight && !t2.isTurnRight) || (!t1.isTurnRight && t2.isTurnRight))
                if (distance < 2*R)
                    return;

            qreal length = 0;
            GeoPoints geoPoints;
            if ( (t1.isTurnRight && !t2.isTurnRight) || (!t1.isTurnRight && t2.isTurnRight) )
                geoPoints = CalculateTwoSideLine(t1, t2, R, length);
            else
                geoPoints = CalculateOneSideLine(t1, t2, R, length);

            maneuverLengths[length] = geoPoints;
        };
        addTurnPairsChecked(t1R, t2R);
        addTurnPairsChecked(t1L, t2R);
        addTurnPairsChecked(t1R, t2L);
        addTurnPairsChecked(t1L, t2L);

        if (maneuverLengths.empty())
            return GeoPoints();

        return maneuverLengths.begin()->second;
    }


    static TurnPointData GetTurnPointData(qreal R, const GeoPoint &pnt, Azimuth az, bool isTurnRight) {
        auto turnPoint = pnt.atDistanceAndAzimuth(R, az + (isTurnRight ? 90 : 270));
        return TurnPointData{turnPoint, isTurnRight};
    }

    GeoPoints CalculateOneSideLine(auto &&turn1, auto &&turn2, qreal R, qreal &length) {
        auto az = Azimuth(turn1.center.azimuthTo(turn2.center) + (turn1.isTurnRight ? 270 : 90));
        auto t1mnv = turn1.center.atDistanceAndAzimuth(R, az);
        auto t2mnv = turn2.center.atDistanceAndAzimuth(R, az);

        GeoPoints points;

        auto azt1p1 = turn1.center.azimuthTo(pnt1_);
        auto azt1t1mnv = az;
        AddManeuverTurn(points, R, turn1, azt1p1, azt1t1mnv);
        length += CalculateManeuverLength(R, turn1.isTurnRight, azt1p1, azt1t1mnv);

        points.push_back(t1mnv);
        points.push_back(t2mnv);
        length += t1mnv.distanceTo(t2mnv);

        auto azt2p1 = turn2.center.azimuthTo(pnt2_);
        auto azt2t2mnv = az;
        AddManeuverTurn(points, R, turn2, azt2t2mnv, azt2p1);
        length += CalculateManeuverLength(R, turn2.isTurnRight, azt2t2mnv, azt2p1);

        return points;
    }
    GeoPoints CalculateTwoSideLine(auto &&turn1, auto &&turn2, qreal R, qreal &length) {
        auto halfDistance = turn1.center.distanceTo(turn2.center) / 2.0;
        auto deltaAz = R2D(acos(R/halfDistance));
        auto az = turn1.center.azimuthTo(turn2.center) + (turn1.isTurnRight ? -deltaAz : deltaAz);
        auto t1mnv = turn1.center.atDistanceAndAzimuth(R, az);
        auto t2mnv = turn2.center.atDistanceAndAzimuth(R, az + 180);

        GeoPoints points;

        auto azt1p1 = turn1.center.azimuthTo(pnt1_);
        auto azt1t1mnv = az;
        AddManeuverTurn(points, R, turn1, azt1p1, azt1t1mnv);
        length += CalculateManeuverLength(R, turn1.isTurnRight, azt1p1, azt1t1mnv);

        points.push_back(t1mnv);
        points.push_back(t2mnv);
        length += t1mnv.distanceTo(t2mnv);

        auto azt2p1 = turn2.center.azimuthTo(pnt2_);
        auto azt2t2mnv = az + 180;
        AddManeuverTurn(points, R, turn2, azt2t2mnv, azt2p1);
        length += CalculateManeuverLength(R, turn2.isTurnRight, azt2t2mnv, azt2p1);

        return points;
    }

    void AddManeuverTurn(GeoPoints &points, qreal R, auto &&turn, Azimuth az1A, Azimuth az2A) {
        if (az1A == az2A)
            return;

        auto addPointsInternal = [&points, R](const GeoPoint &center, auto az1, auto az2, auto delta, auto comparer) {
            for (; comparer(az1, az2); az1 += delta) {// (turn.isTurnRight ? (az < az2) : (az > az2)); az += delta) {
                points.push_back(center.atDistanceAndAzimuth(R, az1));
            }
       };

        const auto delta = 10;
        Azimuth::ValueType az1 = az1A;
        if (az1 < 0)
            az1 += 360;
        Azimuth::ValueType az2 = az2A;
        if (az2 < 0)
            az2 += 360;

        if (turn.isTurnRight) {
            if (az2 < az1)
                az2 += 360;
            addPointsInternal(turn.center, az1, az2, delta, [](auto cur, auto end) { return cur < end; });
        }
        else {
            if (az2 > az1)
                az2 -= 360;
            addPointsInternal(turn.center, az1, az2, -delta, [](auto cur, auto end) { return cur > end; });
        }
    }

    void AddCircle(GeoPoints &points, const GeoPoint &center, qreal R) {
        for (qreal az = 0; az < 360; az += 30)
            points.push_back(center.atDistanceAndAzimuth(R, az));
        points.push_back(points.front());
    }

    static qreal CalculateManeuverLength(qreal R, bool isClockwise, Azimuth az1A, Azimuth az2A) {
        Azimuth::ValueType az1 = az1A;
        if (az1 < 0)
            az1 += 360;
        Azimuth::ValueType az2 = az2A;
        if (az2 < 0)
            az2 += 360;

        Azimuth::ValueType  delta = 0;
        if (isClockwise) {
            if (az2 < az1)
                az2 += 360;
            delta = az2 - az1;
        }
        else {
            if (az2 > az1)
                az2 -= 360;
            delta = az1 - az2;
        }
        return M_PI*R*delta/180;
    }

    const GeoPoint pnt1_;
    const Azimuth az1_;
    const GeoPoint pnt2_;
    const Azimuth az2_;

    const qreal distance12_;
    const Azimuth az12_;
};

}

#endif // PHOTOGEOUCALC_H
