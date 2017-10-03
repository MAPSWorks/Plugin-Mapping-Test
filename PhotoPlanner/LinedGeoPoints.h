#ifndef LINEDGEOPOINTS_H
#define LINEDGEOPOINTS_H

#include "PhotoPlannerCore.h"

namespace aero_photo {

class LinedGeoPoints : public QVector<GeoPoints> {
public:
    explicit LinedGeoPoints() { }
    explicit LinedGeoPoints(size_t size)
        : QVector<GeoPoints>(size)
        , azimuths_(size)
    {
    }

    void SetAzimuth(size_t index, qreal azimuth) { azimuths_[index] = azimuth; }
    qreal GetAzimuth(size_t index) const { return azimuths_[index]; }
private:
    void resize(int asize) = delete;
    QVector<qreal> azimuths_;
};

} // aero_photo

#endif // LINEDGEOPOINTS_H

