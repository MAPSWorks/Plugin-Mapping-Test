#ifndef PHOTOUAVMODEL_H
#define PHOTOUAVMODEL_H

#include <cmath>

namespace aero_photo {

class PhotoUavModel {
public:
    PhotoUavModel(qreal photoVelocityMPerSec, qreal photoRollRadian): velocity_(photoVelocityMPerSec), roll_(photoRollRadian) {}

    qreal GetManeuverR() const { return velocity_ * velocity_ / 9.81 / tan(roll_); }
    qreal velocity() const { return velocity_; }

private:
    qreal velocity_;
    qreal roll_;
};

} // aero_photo

#endif // PHOTOUAVMODEL_H

