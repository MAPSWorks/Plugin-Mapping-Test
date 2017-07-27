#ifndef PHOTOPLANNER_H
#define PHOTOPLANNER_H

#include "PhotoPlannerCore.h"

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

class PhotoPlanner
{

public:
    PhotoPlanner();

//    bool Calculate(const AreaPhotoRegion &area, const Requirements &reqs) {
//        auto area.border_.boundingRect();
//    }

private:

};

}

#endif // PHOTOPLANNER_H
