#ifndef PHOTOPLAN_H
#define PHOTOPLAN_H

#include <QObject>
#include <QGeoCoordinate>
#include <QVariantList>
#include <QVector>

#include "PhotoPlannerMath/PhotoPlanner.h"
#include <memory>


class PhotoPlan : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString cameraModel READ cameraModel WRITE setCameraModel NOTIFY cameraModelChanged)
    Q_PROPERTY(quint32 focusRange READ focusRange WRITE setFocusRange NOTIFY focusRangeChanged)
    Q_PROPERTY(qreal cameraLx READ cameraLx WRITE setCameraLx NOTIFY cameraLxChanged)
    Q_PROPERTY(qreal cameraLy READ cameraLy WRITE setCameraLy NOTIFY cameraLyChanged)
    Q_PROPERTY(quint32 longitOverlap READ longitOverlap WRITE setLongitOverlap NOTIFY longitOverlapChanged)
    Q_PROPERTY(quint32 transverseOverlap READ transverseOverlap WRITE setTransverseOverlap NOTIFY transverseOverlapChanged)
    Q_PROPERTY(quint32 azimuth READ azimuth WRITE setAzimuth NOTIFY azimuthChanged)
    Q_PROPERTY(quint32 altitude READ altitude WRITE setAltitude NOTIFY altitudeChanged)
    Q_PROPERTY(quint32 gsd READ gsd WRITE setGsd NOTIFY gsdChanged)
    Q_PROPERTY(quint32 speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(quint32 width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(quint32 maxRoll READ maxRoll WRITE setMaxRoll NOTIFY maxRollChanged)

    Q_PROPERTY(QVariantList track READ track NOTIFY trackChanged)
    Q_PROPERTY(QVariantList photoCenters READ photoCenters NOTIFY photoCentersChanged)
    Q_PROPERTY(QVariantList photoPrints READ photoPrints NOTIFY photoPrintsChanged)
    Q_PROPERTY(QVariantList trackMarkers READ trackMarkers NOTIFY trackMarkersChanged)

    Q_PROPERTY(QVariantList aoi READ aoi NOTIFY aoiChanged)
    Q_PROPERTY(QString missionType READ missionType)

public:
    explicit PhotoPlan(QObject *parent = nullptr);


    Q_INVOKABLE void calcLinearPhotoPrints(QVariantList aoi);
    Q_INVOKABLE void calcAreaPhotoPrints(QVariantList aoi);
    Q_INVOKABLE void saveFlightPlan(QVariant fileurl);

    Q_INVOKABLE QVariantList loadAoi(QVariant fileurl);
    Q_INVOKABLE void saveAoi(QVariant fileurl, QString poiType, QVariantList aoi);

    Q_INVOKABLE QGeoCoordinate getFlightPointCoord(int index);
    Q_INVOKABLE quint8 getFlightPointType(int index);
    Q_INVOKABLE quint32 getFlightPointCount();

    QString cameraModel() const;
    void setCameraModel(const QString &cameraModel);

    quint32 focusRange() const;
    void setFocusRange(const quint32 &focusRange);

    qreal cameraLx() const;
    void setCameraLx(const qreal &value);

    qreal cameraLy() const;
    void setCameraLy(const qreal &value);

    quint32 longitOverlap() const;
    void setLongitOverlap(const quint32 &longitOverlap);

    quint32 transverseOverlap() const;
    void setTransverseOverlap(const quint32 &transverseOverlap);

    quint32 azimuth() const;
    void setAzimuth(const quint32 &azimuth);

    quint32 altitude() const;
    void setAltitude(const quint32 &altitude);

    quint32 gsd() const;
    void setGsd(const quint32 &gsd);

    quint32 speed() const;
    void setSpeed(const quint32 &speed);

    quint32 width() const;
    void setWidth(const quint32 &width);

    quint32 maxRoll() const;
    void setMaxRoll(const quint32 &maxRoll);

    QVariantList track();

    QVariantList photoCenters();

    QVariantList photoPrints();

    QVariantList trackMarkers();

    QVariantList aoi();
    QString missionType() const;

signals:

    void cameraModelChanged();
    void focusRangeChanged();
    void cameraLxChanged();
    void cameraLyChanged();
    void longitOverlapChanged();
    void transverseOverlapChanged();
    void azimuthChanged();
    void altitudeChanged();
    void gsdChanged();
    void speedChanged();
    void widthChanged();
    void maxRollChanged();
    void trackChanged();
    void photoCentersChanged();
    void photoPrintsChanged();
    void trackMarkersChanged();
    void aoiChanged();

public slots:

private:
    aero_photo::PhotoUavModel CreatePhotoUavModelFromGui() const;
    aero_photo::PhotoCameraModel CreatePhotoCameraModelFromGui() const;

    QString m_cameraModel;
    quint32 m_focusRange;
    qreal m_cameraLx;
    qreal m_cameraLy;
    quint32 m_longitOverlap;
    quint32 m_transverseOverlap;
    quint32 m_azimuth;
    quint32 m_altitude;
    quint32 m_gsd;
    quint32 m_speed;
    quint32 m_width;
    quint32 m_maxRoll;

    QVector<QGeoCoordinate> m_photoCenters;
    QVector<QGeoCoordinate> m_sourceTrack;
    QVector<QGeoCoordinate> m_photoPrints;
    aero_photo::FlightPoints m_flightPoints;
    QVector<QGeoCoordinate> m_aoi;
    QString m_missionType;

    std::unique_ptr<aero_photo::PhotoPlanner> m_apPhotoPlanner;
    void UpdatePhotoPlannerDraw();
};

#endif // PHOTOPLAN_H
