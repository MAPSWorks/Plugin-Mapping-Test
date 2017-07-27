#include "photoplanmodel.h"

PhotoPlanModel::PhotoPlanModel(QObject *parent) : QAbstractListModel(parent)
{

}

void PhotoPlanModel::addGeoCoordinate(const double latitude, const double longitude)
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
    if(m_GeoCoordinates.last() != m_GeoCoordinates.first())
    {
        m_GeoCoordinates.append(m_GeoCoordinates.first());
    }
}

