#ifndef PHOTOPLAN_H
#define PHOTOPLAN_H

#include <QObject>
#include <QGeoCoordinate>
#include <QVariantList>
#include <QVector>

#include "PhotoPlanner/PhotoPlannerMath/PhotoPlanner.h"
#include <memory>


class PhotoPlan : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString cameraModelName READ cameraModelName WRITE setCameraModelName NOTIFY cameraModelNameChanged)
    Q_PROPERTY(QString uavModelName READ uavModelName WRITE setUavModelName NOTIFY uavModelNameChanged)
    Q_PROPERTY(bool enlargeEntryRequired READ enlargeEntryRequired WRITE setEnlargeEntryRequired NOTIFY enlargeEntryRequiredChanged)
    Q_PROPERTY(bool maneuverAligmentRequired READ maneuverAligmentRequired WRITE setManeuverAligmentRequired NOTIFY maneuverAligmentRequiredChanged)

    Q_PROPERTY(quint32 focusRange READ focusRange WRITE setFocusRange NOTIFY focusRangeChanged)
    Q_PROPERTY(qreal cameraLx READ cameraLx WRITE setCameraLx NOTIFY cameraLxChanged)
    Q_PROPERTY(qreal cameraLy READ cameraLy WRITE setCameraLy NOTIFY cameraLyChanged)
    Q_PROPERTY(qreal cameraAx READ cameraAx WRITE setCameraAx NOTIFY cameraAxChanged)
    Q_PROPERTY(qreal cameraAy READ cameraAy WRITE setCameraAy NOTIFY cameraAyChanged)
    Q_PROPERTY(quint32 longitOverlap READ longitOverlap WRITE setLongitOverlap NOTIFY longitOverlapChanged)
    Q_PROPERTY(quint32 transverseOverlap READ transverseOverlap WRITE setTransverseOverlap NOTIFY transverseOverlapChanged)
    Q_PROPERTY(quint32 azimuth READ azimuth WRITE setAzimuth NOTIFY azimuthChanged)
    Q_PROPERTY(quint32 altitude READ altitude WRITE setAltitude NOTIFY altitudeChanged)
    Q_PROPERTY(qreal gsd READ gsd WRITE setGsd NOTIFY gsdChanged)
    Q_PROPERTY(quint32 speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(qreal maxRoll READ maxRoll WRITE setMaxRoll NOTIFY maxRollChanged)
    Q_PROPERTY(qreal uavManeuverR READ uavManeuverR WRITE setUavManeuverR NOTIFY uavManeuverRChanged)

    Q_PROPERTY(quint32 linearWidth READ linearWidth WRITE setLinearWidth NOTIFY linearWidthChanged)
    Q_PROPERTY(quint32 linearRuns READ linearRuns WRITE setLinearRuns NOTIFY linearRunsChanged)

    Q_PROPERTY(QVariantList track READ track NOTIFY trackChanged)
    Q_PROPERTY(QVariantList photoCenters READ photoCenters NOTIFY photoCentersChanged)
    Q_PROPERTY(QVariantList photoPrints READ photoPrints NOTIFY photoPrintsChanged)
    Q_PROPERTY(QVariantList trackMarkers READ trackMarkers NOTIFY trackMarkersChanged)

    Q_PROPERTY(QVariantList aoi READ aoi NOTIFY aoiChanged)
    Q_PROPERTY(QString missionType READ missionType)

public:
    explicit PhotoPlan(QObject *parent = nullptr);

    Q_INVOKABLE quint32 calcPhotoPlanRuns();
    Q_INVOKABLE quint32 calcPhotoPlanWidth();

    Q_INVOKABLE qreal calcPhotoParamGsdOnAltitude();
    Q_INVOKABLE qreal calcPhotoParamAltitudeOnGsd();

    Q_INVOKABLE qreal calcUavManeuverROnMaxRoll();
    Q_INVOKABLE qreal calcUavMaxRollOnManeuverR();

    Q_INVOKABLE void calcLinearPhotoPrints(QVariantList aoi);
    Q_INVOKABLE void calcAreaPhotoPrints(QVariantList aoi);
    Q_INVOKABLE void saveFlightPlan(QVariant fileurl, int pointsInPacket);

    Q_INVOKABLE QVariantList loadAoi(QVariant fileurl);
    Q_INVOKABLE void saveAoi(QVariant fileurl, QString poiType, QVariantList aoi);

    Q_INVOKABLE QGeoCoordinate getFlightPointCoord(int index);
    Q_INVOKABLE quint8 getFlightPointType(int index);
    Q_INVOKABLE quint32 getFlightPointCount();

    QString cameraModelName() const;
    void setCameraModelName(const QString &value);

    QString uavModelName() const;
    void setUavModelName(const QString &value);

    bool enlargeEntryRequired() const;
    void setEnlargeEntryRequired(const bool value);

    bool maneuverAligmentRequired() const;
    void setManeuverAligmentRequired(const bool value);

    quint32 focusRange() const;
    void setFocusRange(const quint32 value);

    qreal cameraLx() const;
    void setCameraLx(const qreal value);

    qreal cameraLy() const;
    void setCameraLy(const qreal value);

    qreal cameraAx() const;
    void setCameraAx(const qreal value);

    qreal cameraAy() const;
    void setCameraAy(const qreal value);

    quint32 longitOverlap() const;
    void setLongitOverlap(const quint32 value);

    quint32 transverseOverlap() const;
    void setTransverseOverlap(const quint32 value);

    quint32 azimuth() const;
    void setAzimuth(const quint32 value);

    quint32 altitude() const;
    void setAltitude(const quint32 value);

    qreal gsd() const;
    void setGsd(const qreal value);

    quint32 speed() const;
    void setSpeed(const quint32 value);

    quint32 linearWidth() const;
    void setLinearWidth(const quint32 value);

    quint32 linearRuns() const;
    void setLinearRuns(const quint32 value);

    qreal maxRoll() const;
    void setMaxRoll(const qreal value);

    qreal uavManeuverR() const;
    void setUavManeuverR(const qreal value);

    QVariantList track();

    QVariantList photoCenters();

    QVariantList photoPrints();

    QVariantList trackMarkers();

    QVariantList aoi();
    QString missionType() const;

signals:

    void cameraModelNameChanged();
    void uavModelNameChanged();
    void enlargeEntryRequiredChanged();
    void maneuverAligmentRequiredChanged();
    void focusRangeChanged();
    void cameraLxChanged();
    void cameraLyChanged();
    void cameraAxChanged();
    void cameraAyChanged();
    void longitOverlapChanged();
    void transverseOverlapChanged();
    void azimuthChanged();
    void altitudeChanged();
    void gsdChanged();
    void speedChanged();
    void linearWidthChanged();
    void linearRunsChanged();
    void maxRollChanged();
    void uavManeuverRChanged();
    void trackChanged();
    void photoCentersChanged();
    void photoPrintsChanged();
    void trackMarkersChanged();
    void aoiChanged();

public slots:

private:
    aero_photo::PhotoUavModel CreatePhotoUavModelFromGui() const;
    aero_photo::PhotoCameraModel CreatePhotoCameraModelFromGui() const;
    void UpdateCalculationParamsFromGui() const;

    QString m_cameraModelName;
    QString m_uavModelName;
    bool m_enlargeEntryRequired = false;
    bool m_maneuverAligmentRequired = true;
    quint32 m_focusRange;
    qreal m_cameraLx;
    qreal m_cameraLy;
    qreal m_cameraAx;
    qreal m_cameraAy;
    quint32 m_longitOverlap;
    quint32 m_transverseOverlap;
    quint32 m_azimuth;
    quint32 m_altitude;
    qreal m_gsd;
    quint32 m_speed;
    qreal m_maxRoll;
    qreal m_maneuverR;

    quint32 m_linearWidth = 100;
    quint32 m_linearRuns = 1;


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
