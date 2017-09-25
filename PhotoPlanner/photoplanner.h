#ifndef PHOTOPLANNER_H
#define PHOTOPLANNER_H

#include "PhotoPlannerCore.h"
#include "photocamera.h"
#include "PhotoPrintsGenerator.h"

#include <QPoint>
#include <QtDebug>

namespace aero_photo {

using CartesianPoint = QPointF;

//class Requirements {
//public:
//    double lx_ = 0;
//    double ly_ = 0;
//    double altitude_ = 0;
//};

//class Conditions {
//public:
//    double windSpeed = 0;
//    double windBearing = 0;
//};

class FlightPoint : public GeoPoint {
public:
    FlightPoint() {}
    FlightPoint(const GeoPoint &geoPoint, int type)
        : GeoPoint(geoPoint)
        , type_(type)
    {
    }

    void setType(int type) { type_ = type; }
    int type() const { return type_; }

private:
    int type_ = 0;
};

using FlightPoints = QVector<FlightPoint>;


class PhotoPlanner {
protected:
    PhotoPlanner(const PhotoUavModel &photoUav, const PhotoCameraModel &photoCamera)
        : photoUav_(photoUav)
        , photoCamera_(photoCamera) {
    }

public:
    virtual ~PhotoPlanner() {}

    bool IsCalculated() const { return isCalculated; }
    const PhotoPrints &GetPhotoPrints() const { return photoPrints_; }
    const GeoPoints &GetTrackPoints() const { return trackPoints_; }
    const FlightPoints &GetFlightPoints() const  { return flightPoints_; }

protected:
    void CalculateTrack(double h) {
        trackPoints_.clear();
        flightPoints_.clear();

        int prevLine = -1;
        for(int i = 0; i<linedGeoPoints_.size(); ++i) {
            auto &line = linedGeoPoints_[i];
            if(line.empty())
                continue;

            if(flightPoints_.empty()) {
                const auto R = photoUav_.GetManeuverR();
                if(R!=0) {
                    auto startPointAt3R = linedGeoPoints_[i].front().atDistanceAndAzimuth(4*R, linedGeoPoints_.GetAzimuth(i) + 180);
                    auto startPointAt1R = linedGeoPoints_[i].front().atDistanceAndAzimuth(2*R, linedGeoPoints_.GetAzimuth(i) + 180);
                    flightPoints_.push_back(FlightPoint(startPointAt3R, 0));
                    flightPoints_.push_back(FlightPoint(startPointAt1R, 1));
                }
            }

            if(prevLine>=0) {
                ManeuverTrackAlignment aligment(linedGeoPoints_[prevLine].back(), linedGeoPoints_.GetAzimuth(prevLine), linedGeoPoints_[i].front(), linedGeoPoints_.GetAzimuth(i));
                auto aligmentPoints = aligment.Calculate(photoUav_);
                for(auto point : aligmentPoints)
                    trackPoints_.push_back(point);
                for(auto point : aligment.GetFlightPoints())
                    flightPoints_.push_back(FlightPoint(point, 0));
            }

            trackPoints_.push_back(line.front());
            flightPoints_.push_back(FlightPoint(line.front(), 0));
            flightPoints_.back().setAltitude(h);
            if(line.size()>1) {
                trackPoints_.push_back(line.back());
                flightPoints_.push_back(FlightPoint(line.back(), 1));
                flightPoints_.back().setAltitude(h);
            }
            prevLine = i;
        }

        isCalculated = true;
    }

    bool isCalculated = false;
    PhotoUavModel photoUav_;
    PhotoCameraModel photoCamera_;
    LinedGeoPoints linedGeoPoints_;
    GeoPoints trackPoints_;
    FlightPoints flightPoints_;
    PhotoPrints photoPrints_;
};

class LinearPhotoPlanner : public PhotoPlanner {
public:
    LinearPhotoPlanner(const PhotoUavModel &photoUav, const PhotoCameraModel &photoCamera, const LinearPhotoRegion &photoRegion)
        : PhotoPlanner(photoUav, photoCamera)
        , photoPrintsGenerator_(photoRegion) {
        qDebug() << "Created photo planner for linear region: " << photoRegion.GetTrack();
    }

    bool Calculate(double h, double Px, double Py, size_t totalRuns) {
        isCalculated = false;
        double Bx, By;
        photoCamera_.CalcBxBy(h, Px, Py, Bx, By);
        linedGeoPoints_ = photoPrintsGenerator_.GeneratePhotoPrintsCenters(Bx, By, totalRuns);
        double Lx, Ly;
        photoCamera_.CalcLxLy(h, Lx, Ly);
        photoPrints_ = photoPrintsGenerator_.GeneratePhotoPrints(linedGeoPoints_, Lx, Ly);
        CalculateTrack(h);
        return IsCalculated();
    }

private:
    LinearPhotoPrintsGenerator photoPrintsGenerator_;
};

class AreaPhotoPlanner : public PhotoPlanner {
public:
    AreaPhotoPlanner(const PhotoUavModel &photoUav, const PhotoCameraModel &photoCamera, const AreaPhotoRegion &photoRegion)
        : PhotoPlanner(photoUav, photoCamera)
        , photoPrintsGenerator_(photoRegion) {
        qDebug() << "Created photo planner for area region: " << photoRegion.GetBorder();
    }

    bool Calculate(double h, double Px, double Py, qreal azimuth, size_t extentBorderValue) {
        isCalculated = false;
        double Bx, By;
        photoCamera_.CalcBxBy(h, Px, Py, Bx, By);
        linedGeoPoints_ = photoPrintsGenerator_.GeneratePhotoPrintsCenters(Bx, By, azimuth, extentBorderValue);
        double Lx, Ly;
        photoCamera_.CalcLxLy(h, Lx, Ly);
        photoPrints_ = photoPrintsGenerator_.GeneratePhotoPrints(linedGeoPoints_, Lx, Ly);

        CalculateTrack(h);

        return IsCalculated();
    }

private:
    AreaPhotoPrintsGenerator photoPrintsGenerator_;
};



}

#endif // PHOTOPLANNER_H
