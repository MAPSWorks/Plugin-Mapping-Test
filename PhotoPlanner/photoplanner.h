#ifndef PHOTOPLANNER_H
#define PHOTOPLANNER_H

#include "PhotoPlannerCore.h"
#include "photocamera.h"
#include "PhotoPrintsGenerator.h"

#include <QPoint>

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

class PhotoPlanner {
protected:
    PhotoPlanner(const PhotoUavModel &photoUav, const PhotoCameraModel &photoCamera)
        : photoUav_(photoUav)
        , photoCamera_(photoCamera) {
    }

public:
    bool IsCalculated() const { return isCalculated; }
    const PhotoPrints &GetPhotoPrints() const { return photoPrints_; }
    const GeoPoints &GetTrackPoints() const { return trackPoints_; }

protected:
    void CalculateTrack() {
        trackPoints_.clear();

        int prevLine = -1;
        for(int i = 0; i<linedGeoPoints_.size(); ++i) {
            auto &line = linedGeoPoints_[i];
            if(line.empty())
                continue;

            if(prevLine>=0) {
                ManeuverTrackAlignment aligment(linedGeoPoints_[prevLine].back(), linedGeoPoints_.GetAzimuth(prevLine), linedGeoPoints_[i].front(), linedGeoPoints_.GetAzimuth(i));
                auto aligmentPoints = aligment.Calculate(photoUav_);
                for(auto point : aligmentPoints)
                    trackPoints_.push_back(point);
            }

            trackPoints_.push_back(line.front());
            if(line.size()>1)
                trackPoints_.push_back(line.back());
            prevLine = i;
        }

        isCalculated = true;
    }

    bool isCalculated = false;
    PhotoUavModel photoUav_;
    PhotoCameraModel photoCamera_;
    LinedGeoPoints linedGeoPoints_;
    GeoPoints trackPoints_;
    PhotoPrints photoPrints_;
};

class LinearPhotoPlanner : public PhotoPlanner {
public:
    LinearPhotoPlanner(const PhotoUavModel &photoUav, const PhotoCameraModel &photoCamera, const LinearPhotoRegion &photoRegion)
        : PhotoPlanner(photoUav, photoCamera)
        , photoPrintsGenerator_(photoRegion) {
    }

    bool Calculate(double h, double Px, double Py, size_t totalRuns) {
        isCalculated = false;
        double Bx, By;
        photoCamera_.CalcBxBy(h, Px, Py, Bx, By);
        linedGeoPoints_ = photoPrintsGenerator_.GeneratePhotoPrintsCenters(Bx, By, totalRuns);
        double Lx, Ly;
        photoCamera_.CalcLxLy(h, Lx, Ly);
        photoPrints_ = photoPrintsGenerator_.GeneratePhotoPrints(linedGeoPoints_, Lx, Ly);

        CalculateTrack();

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
    }

    bool Calculate(double h, double Px, double Py, qreal azimuth, size_t extentBorderValue) {
        isCalculated = false;
        double Bx, By;
        photoCamera_.CalcBxBy(h, Px, Py, Bx, By);
        linedGeoPoints_ = photoPrintsGenerator_.GeneratePhotoPrintsCenters(Bx, By, azimuth, extentBorderValue);
        double Lx, Ly;
        photoCamera_.CalcLxLy(h, Lx, Ly);
        photoPrints_ = photoPrintsGenerator_.GeneratePhotoPrints(linedGeoPoints_, Lx, Ly);

        CalculateTrack();

        return IsCalculated();
    }

private:
    AreaPhotoPrintsGenerator photoPrintsGenerator_;
};



}

#endif // PHOTOPLANNER_H
