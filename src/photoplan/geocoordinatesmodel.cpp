#include "geocoordinatesmodel.h"

GeoCoordinatesModel::GeoCoordinatesModel(QObject *parent) : QAbstractListModel(parent)
{

}

void GeoCoordinatesModel::addGeoCoordinate(const double latitude, const double longitude)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_GeoCoordinates << QGeoCoordinate(latitude, longitude);
    endInsertRows();
}

void GeoCoordinatesModel::addGeoCoordinate(const QGeoCoordinate& coord)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_GeoCoordinates << coord;
    endInsertRows();
}

int GeoCoordinatesModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_GeoCoordinates.count();
}


QVariant GeoCoordinatesModel::data(const QModelIndex & index, int role) const {
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

QHash<int, QByteArray> GeoCoordinatesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[LatitudeRole] = "latitude";
    roles[LongitudeRole] = "longitude";
    roles[AltitudeRole] = "altitude";
    return roles;
}

QVector<QGeoCoordinate> GeoCoordinatesModel::getGeoCoordinates() const
{
    return m_GeoCoordinates;
}

QVariantMap GeoCoordinatesModel::get(int row) {
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
