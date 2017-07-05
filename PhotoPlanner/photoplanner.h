#ifndef PHOTOPLANNER_H
#define PHOTOPLANNER_H

#include "photoplannercore.h"

#include <QPoint>

namespace aero_photo {

using CartesianPoint = QPointF;

class Requirements {
public:
    double lx_ = 0;
    double ly_ = 0;
    double altitude_ = 0;
};

class Conditions {
public:
    double windSpeed = 0;
    double windBearing = 0;
};

//class PhotoPrintsGenerator {
//public:
//    PhotoPrintsGenerator(const TargetArea &area) : targetArea_(area) {
//        if (area.border_.size()<3)
//            throw std::logic_error("Invalid TargetArea size()");

//        // azimuthOfPhotography_ = area.border_[0].azimuthTo(area.border_[1]);
//        targetArea_.reserve(area.border_.size());
//        for(auto geoPoint : area) {
//          targetArea_ << QPointF(geoPoint.latitude, geoPoint.longitude);
//        }
//        boundingRect_ = targetArea_.boundingRect();
//        boundingRadius_ = CalculateBoundingRadius();
//        geoCenter_ = CalculateGeoCenter();
//    }

//    GenerateLines(qreal azimuth, qreal Lx, qreal Ly) {
//        qreal azimuthUp = azimuth
//        for (qreal prodY = 0; true; ++prodY) {
//            qreal offset = Ly * prodY;
//        }
//    }

//private:
//    CalculateLine(const GeoPoint &geoPoint, qreal lineAzimuth) {
//        QLinkedList<GeoPoint> lineOfPhotoPrints;

//        auto processDistance = [this, &geoPoint, &lineOfPhotoPrints, ](qreal prod){
//            qreal distance = prod * Lx_;
//            auto distanceGeoPoint = geoPoint.atDistanceAndAzimuth(distance, azimuthOfPhotography_);
//            QPointF distancePoint(distancePoint.latitude(), distancePoint.longitude());
//            if (!boundingRect_.contains(distancePoint))
//                return false;
//            if (targetArea_.containsPoint(distancePoint, Qt::FillRule::OddEvenFill))
//                lineOfPhotoPrints.push_back(distanceGeoPoint);
//            return true;
//        };

//        for(qreal prod = 0; processDistance(prod); ++prod) {
//        }

//        for(qreal prod = 0; processDistance(prod); ++prod) {
//        }
//    }

//    QPolygonF targetArea_;
//    QRectF boundingRect_;
//    qreal boundingRadius_;
//    GeoPoint geoCenter_;

//    //qreal azimuthOfPhotography_;
//    //qreal Lx_, Ly_;
//};

class PhotoPlanner
{

public:
    PhotoPlanner();

    bool Calculate(const TargetArea &area, const Requirements &reqs) {
//        auto area.border_.boundingRect();
    }

private:

};

}

#endif // PHOTOPLANNER_H
