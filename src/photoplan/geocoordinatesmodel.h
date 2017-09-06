#ifndef GEOCOORDINATESMODEL_H
#define GEOCOORDINATESMODEL_H

#include <QAbstractListModel>
#include <QGeoCoordinate>
#include "PhotoPlanner/PhotoPlannerCore.h"

class GeoCoordinatesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum QGeoCoordinateRoles{
        LatitudeRole = Qt::UserRole + 1,
        LongitudeRole,
        AltitudeRole
    };

    GeoCoordinatesModel(QObject *parent=0);

    Q_INVOKABLE void addGeoCoordinate(const double latitude, const double longitude);
    Q_INVOKABLE void addGeoCoordinate(const QGeoCoordinate& coord);
    Q_INVOKABLE QVariantMap get(int row);
    Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVector<QGeoCoordinate> getGeoCoordinates() const;

    void clear();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QVector<QGeoCoordinate> m_GeoCoordinates;
};
/*
class PhotoPrintsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PhotoPrintsRoles{
        CenterRole = Qt::UserRole + 1,
        CenterRole
    };

    PhotoPrintsModel(QObject *parent=0);


private:
    QVector<aero_photo::PhotoPrint> m_photoPrints;
};
*/
#endif // GEOCOORDINATESMODEL_H
