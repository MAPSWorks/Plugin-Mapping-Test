#ifndef PHOTOPLANMODEL_H
#define PHOTOPLANMODEL_H

#include <QAbstractListModel>
#include <QGeoCoordinate>

class PhotoPlanModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum QGeoCoordinateRoles{
        LatitudeRole = Qt::UserRole + 1,
        LongitudeRole,
        AltitudeRole
    };

    PhotoPlanModel(QObject *parent=0);

    void addGeoCoordinate(const QGeoCoordinate &geoCoordinate);
    Q_INVOKABLE void append(const QGeoCoordinate &geoCoordinate);
    Q_INVOKABLE void append1(const double latitude, const double longitude);
    Q_INVOKABLE QVariantMap get(int row);
    Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE void recalc();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<QGeoCoordinate> m_GeoCoordinates;
};

#endif // PHOTOPLANMODEL_H
