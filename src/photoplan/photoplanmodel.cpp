#include "photoplanmodel.h"

#include "PhotoPlanner/PhotoPrintsGenerator.h"

PhotoPlanModel::PhotoPlanModel(QObject *parent) : QAbstractListModel(parent)
{

}

void PhotoPlanModel::addGeoCoordinate(const QGeoCoordinate &geoCoordinate)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_GeoCoordinates << geoCoordinate;
    endInsertRows();
}

void PhotoPlanModel::append(const QGeoCoordinate &geoCoordinate)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_GeoCoordinates << geoCoordinate;
    endInsertRows();
}

void PhotoPlanModel::append1(const double latitude, const double longitude)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_GeoCoordinates << QGeoCoordinate(latitude, longitude);
    endInsertRows();
}

int PhotoPlanModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_GeoCoordinates.count();
}


QVariant PhotoPlanModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_GeoCoordinates.count())
        return QVariant();

    const QGeoCoordinate &geoCoordinate = m_GeoCoordinates[index.row()];
    if (role == LatitudeRole)
        return geoCoordinate.latitude();
    else if (role == LongitudeRole)
        return geoCoordinate.longitude();
    else if (role == AltitudeRole)
        return geoCoordinate.altitude();
    return QVariant();
}

QHash<int, QByteArray> PhotoPlanModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[LatitudeRole] = "latitude";
    roles[LongitudeRole] = "longitude";
    roles[AltitudeRole] = "altitude";
    return roles;
}

QVariantMap PhotoPlanModel::get(int row) {
    QHash<int,QByteArray> names = roleNames();
    QHashIterator<int, QByteArray> i(names);
    QVariantMap res;
    while (i.hasNext()) {
        i.next();
        QModelIndex idx = index(row, 0);
        QVariant data = idx.data(i.key());
        res[i.value()] = data;
        //cout << i.key() << ": " << i.value() << endl;
    }
    return res;
}

void PhotoPlanModel::recalc()
{
    using namespace aero_photo;
    auto getDefaultTrack = []() {
        GeoPoint startPoint(47.2589912414551, 11.3247512741089);
        GeoPoints defaultTrack;
        defaultTrack << startPoint;
        defaultTrack << defaultTrack.back().atDistanceAndAzimuth(1500, 90);
        defaultTrack << defaultTrack.back().atDistanceAndAzimuth(750, 180);
//        defaultTrack << defaultTrack.back().atDistanceAndAzimuth(1100, -90);
//        defaultTrack << defaultTrack.back().atDistanceAndAzimuth(220, 0);
        return defaultTrack;
    };

    auto areaTestFun = [](auto &&track) {
        AreaPhotoRegion testArea(track);
        AreaPhotoPrintsGenerator generator(testArea);
        auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(200, 90, 90);
//        auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(200, 90, generator.GetPreferredAzimuth());
        auto photoPrints = generator.GeneratePhotoPrints(photoPrintsCenters, 250, 120);
        return photoPrints;
    };


    auto linearTestFun = [](auto &&track) {
        LinearPhotoRegion region(track);
        LinearPhotoPrintsGenerator generator(region);
        auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(200, 90, 4);
        auto photoPrints = generator.GeneratePhotoPrints(photoPrintsCenters, 250, 120);
        return photoPrints;
    };

    static int counter = 0;
//    m_GeoCoordinates = getDefaultTrack();

    if (counter++ % 2 == 0) {
        auto photoPrints = areaTestFun(m_GeoCoordinates);
        for (auto photoPrint : photoPrints) {
            //        m_GeoCoordinates.append(photoPrint.GetBorder());
            m_GeoCoordinates.append(photoPrint.GetCenter());
        }
    }
}

