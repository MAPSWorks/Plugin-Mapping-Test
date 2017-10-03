#ifndef PHOTOPLANNER_H
#define PHOTOPLANNER_H

#include "PhotoPlannerCore.h"
#include "PhotoCameraModel.h"
#include "PhotoUavModel.h"
#include "LinedGeoPoints.h"
#include "ManeuverTrackAlignment.h"


namespace aero_photo {

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

    qreal velocity() const { return photoUav_.velocity(); }

protected:
    void CalculateTrack(double Bx) {
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
            flightPoints_.push_back(FlightPoint(line.front(), 0, Bx));
            if(line.size()>1) {
                trackPoints_.push_back(line.back());
                flightPoints_.push_back(FlightPoint(line.back(), 1));
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



}

#endif // PHOTOPLANNER_H