#ifndef PHOTOPLANNERCORE_H
#define PHOTOPLANNERCORE_H

#include <QVector>
#include <QGeoCoordinate>

namespace aero_photo {

    using GeoPoint = QGeoCoordinate; // WGS84
    using GeoPoints = QVector<GeoPoint>;
    using CartesianPoint = QPointF;


class PhotoPrint {
public:
    PhotoPrint() {}
    PhotoPrint(const GeoPoint &center, const GeoPoints &border)
        : center_(center), border_(border) {
    }

    const GeoPoint &GetCenter() const {
        return center_;
    }
    const GeoPoints &GetBorder() const {
        return border_;
    }

private:
    GeoPoint center_;
    GeoPoints border_;
};

using PhotoPrints = QVector<PhotoPrint>;

class LinearPhotoRegion {
public:
    LinearPhotoRegion(const GeoPoints &track) : track_(track) { }

    const GeoPoints &GetTrack() const {
        return track_;
    }

private:
    const GeoPoints track_;
};

class AreaPhotoRegion {
public:
    AreaPhotoRegion(const GeoPoints &border) : border_(border) { }

    const GeoPoints &GetBorder() const {
        return border_;
    }

private:
    const GeoPoints border_;
};

}

#endif // PHOTOPLANNERCORE_H
