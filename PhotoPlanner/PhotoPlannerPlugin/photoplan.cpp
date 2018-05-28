#include "photoplan.h"

#include <QDataStream>
#include <QFile>
#include <QUrl>
#include <QVariant>
#include <QtDebug>

PhotoPlan::PhotoPlan(QObject *parent) : QObject(parent)
{
    m_cameraModelName           = "Sony S600";
    m_enlargeEntryRequired  = false;
    m_focusRange            = 35;
    m_longitOverlap         = 20;
    m_transverseOverlap     = 20;
    m_azimuth               = 0;
    m_altitude              = 100;
    m_gsd                   = 150;
    m_speed                 = 40;
    m_maxRoll               = 30;
}

quint32 PhotoPlan::calcPhotoPlanRuns()
{
    auto photoModel = CreatePhotoCameraModelFromGui();
    return photoModel.CalcLinearRuns(altitude(), longitOverlap(), transverseOverlap(), linearWidth());
}

quint32 PhotoPlan::calcPhotoPlanWidth()
{
    auto photoModel = CreatePhotoCameraModelFromGui();
    return photoModel.CalcLinearWidth(altitude(), longitOverlap(), transverseOverlap(), linearRuns());
}

qreal PhotoPlan::calcPhotoParamGsdOnAltitude() {
    auto photoModel = CreatePhotoCameraModelFromGui();
    return photoModel.CalcGsd(altitude());
}

qreal PhotoPlan::calcPhotoParamAltitudeOnGsd() {
    auto photoModel = CreatePhotoCameraModelFromGui();
    return photoModel.CalcH(gsd());
}

qreal PhotoPlan::calcUavManeuverROnMaxRoll() {
    auto uavModel = CreatePhotoUavModelFromGui();
    return uavModel.CalcUavManeuverROnMaxRoll(aero_photo::D2R(maxRoll()));
}

qreal PhotoPlan::calcUavMaxRollOnManeuverR() {
    auto uavModel = CreatePhotoUavModelFromGui();
    return aero_photo::R2D(uavModel.CalcUavMaxRollOnManeuverR(uavManeuverR()));
}

QString PhotoPlan::cameraModelName() const
{
    return m_cameraModelName;
}
void PhotoPlan::setCameraModelName(const QString &value)
{
    if(value != m_cameraModelName)
    {
        m_cameraModelName = value;
        emit cameraModelNameChanged();
    }
}

QString PhotoPlan::uavModelName() const
{
    return m_uavModelName;
}
void PhotoPlan::setUavModelName(const QString &value)
{
    if(value != m_uavModelName)
    {
        m_uavModelName = value;
        emit uavModelNameChanged();
    }
}


bool PhotoPlan::enlargeEntryRequired() const {
    return m_enlargeEntryRequired;
}

void PhotoPlan::setEnlargeEntryRequired(const bool value) {
    if (value!=m_enlargeEntryRequired) {
        m_enlargeEntryRequired = value;
        emit enlargeEntryRequiredChanged();
    }
}

quint32 PhotoPlan::focusRange() const
{
    return m_focusRange;
}

void PhotoPlan::setFocusRange(const quint32 value)
{
    if(value != m_focusRange)
    {
        m_focusRange = value;
        emit focusRangeChanged();
    }
}

qreal PhotoPlan::cameraLx() const {
    return m_cameraLx;
}
void PhotoPlan::setCameraLx(const qreal value){
    if(value != m_cameraLx)
    {
        m_cameraLx = value;
        emit cameraLxChanged();
    }
}

qreal PhotoPlan::cameraLy() const {
    return m_cameraLy;
}
void PhotoPlan::setCameraLy(const qreal value) {
    if(value != m_cameraLy)
    {
        m_cameraLy = value;
        emit cameraLyChanged();
    }
}

qreal PhotoPlan::cameraAx() const {
    return m_cameraAx;
}
void PhotoPlan::setCameraAx(const qreal value){
    if(value != m_cameraAx)
    {
        m_cameraAx = value;
        emit cameraAxChanged();
    }
}

qreal PhotoPlan::cameraAy() const {
    return m_cameraAy;
}
void PhotoPlan::setCameraAy(const qreal value) {
    if(value != m_cameraAy)
    {
        m_cameraAy = value;
        emit cameraAyChanged();
    }
}

quint32 PhotoPlan::longitOverlap() const
{
    return m_longitOverlap;
}

void PhotoPlan::setLongitOverlap(const quint32 value)
{
    if(value != m_longitOverlap)
    {
        m_longitOverlap = value;
        emit longitOverlapChanged();
    }
}

quint32 PhotoPlan::transverseOverlap() const
{
    return m_transverseOverlap;
}

void PhotoPlan::setTransverseOverlap(const quint32 value)
{
    if(value != m_transverseOverlap)
    {
        m_transverseOverlap = value;
        emit transverseOverlapChanged();
    }
}

quint32 PhotoPlan::azimuth() const
{
    return m_azimuth;
}

void PhotoPlan::setAzimuth(const quint32 value)
{
    if(value != m_azimuth)
    {
        m_azimuth = value;
        emit azimuthChanged();
    }
}

quint32 PhotoPlan::altitude() const
{
    return m_altitude;
}

void PhotoPlan::setAltitude(const quint32 value)
{
    if(value != m_altitude)
    {
        m_altitude = value;
        emit altitudeChanged();
    }
}

qreal PhotoPlan::gsd() const
{
    return m_gsd;
}
void PhotoPlan::setGsd(const qreal value)
{
    if(value != m_gsd)
    {
        m_gsd = value;
        emit gsdChanged();
    }
}

quint32 PhotoPlan::speed() const
{
    return m_speed;
}
void PhotoPlan::setSpeed(const quint32 value)
{
    if(value != m_speed)
    {
        m_speed = value;
        emit speedChanged();
    }
}

quint32 PhotoPlan::linearWidth() const
{
    return m_linearWidth;
}
void PhotoPlan::setLinearWidth(const quint32 value)
{
    if(value != m_linearWidth)
    {
        m_linearWidth = value;
        emit linearWidthChanged();
    }
}

quint32 PhotoPlan::linearRuns() const
{
    return m_linearRuns;
}
void PhotoPlan::setLinearRuns(const quint32 value)
{
    if(value != m_linearRuns)
    {
        m_linearRuns = value;
        emit linearRunsChanged();
    }
}

qreal PhotoPlan::maxRoll() const
{
    return m_maxRoll;
}
void PhotoPlan::setMaxRoll(const qreal value)
{
    if(value != m_maxRoll)
    {
        m_maxRoll = value;
        emit maxRollChanged();
    }
}

qreal PhotoPlan::uavManeuverR() const
{
    return m_maneuverR;
}
void PhotoPlan::setUavManeuverR(const qreal value)
{
    if(value != m_maneuverR)
    {
        m_maneuverR = value;
        emit uavManeuverRChanged();
    }
}


QVariantList PhotoPlan::track()
{
    QVariantList lst;

    foreach (QGeoCoordinate crd, m_sourceTrack) {
        lst.append(QVariant::fromValue(crd));
    }
    return lst;
}

QVariantList PhotoPlan::photoCenters()
{
    QVariantList lst;

    foreach (QGeoCoordinate crd, m_photoCenters) {
        lst.append(QVariant::fromValue(crd));
    }
    return lst;
}

QVariantList PhotoPlan::photoPrints()
{
    QVariantList lst;

    foreach (QGeoCoordinate crd, m_photoPrints) {
        lst.append(QVariant::fromValue(crd));
    }
    return lst;
}

QVariantList PhotoPlan::trackMarkers()
{
    QVariantList lst;

    for(aero_photo::GeoPoint point : m_flightPoints) {
        lst.append(QVariant::fromValue(point));
    }
    return lst;
}

QVariantList PhotoPlan::aoi()
{
    QVariantList lst;
    for(aero_photo::GeoPoint point : m_aoi) {
        lst.append(QVariant::fromValue(point));
    }
    return lst;
}

QString PhotoPlan::missionType() const
{
    return m_missionType;
}

aero_photo::PhotoUavModel PhotoPlan::CreatePhotoUavModelFromGui() const {
    qreal speedMPerSecond = speed();
    auto rollRadian = aero_photo::D2R(maxRoll());
    return aero_photo::PhotoUavModel(speedMPerSecond, rollRadian);
}

aero_photo::PhotoCameraModel PhotoPlan::CreatePhotoCameraModelFromGui() const  {
    qreal focusM = qreal(focusRange()) / 100;
    qreal lxM = qreal(cameraLx()) / 100;
    qreal lyM = qreal(cameraLy()) / 100;
    qreal ax = cameraAx();
    qreal ay = cameraAy();
    return aero_photo::PhotoCameraModel(focusM, lxM, lyM, ax, ay);
}

void PhotoPlan::UpdateCalculationParamsFromGui() const {
    aero_photo::CalculationParams::Instance().enlargeEntryRequired = m_enlargeEntryRequired;
}

#include <algorithm>
#include <iostream>
#include <vector>

#include "PhotoPlanner/PhotoPlannerMath/LinearPhotoPlanner.h"
#include "PhotoPlanner/PhotoPlannerMath/AreaPhotoPlanner.h"


using namespace std;

void PhotoPlan::calcLinearPhotoPrints(QVariantList aoi)
{
    using namespace aero_photo;

    QVector<QGeoCoordinate> pathAoI;

    foreach(QVariant crd, aoi) {
        pathAoI.append(crd.value<QGeoCoordinate>());
    }

    if(pathAoI.empty()) {
        return;
    }

    auto uavModel = CreatePhotoUavModelFromGui();
    auto photoCameraModel = CreatePhotoCameraModelFromGui();
    UpdateCalculationParamsFromGui();
    LinearPhotoRegion region(pathAoI);
    m_apPhotoPlanner.reset(new LinearPhotoPlanner(uavModel, photoCameraModel, region));
    dynamic_cast<LinearPhotoPlanner *>(m_apPhotoPlanner.get())->Calculate(altitude(), longitOverlap(), transverseOverlap(), linearWidth());

    UpdatePhotoPlannerDraw();
}

void PhotoPlan::calcAreaPhotoPrints(QVariantList aoi)
{
    using namespace aero_photo;

    QVector<QGeoCoordinate> pathAoI;

    foreach(QVariant crd, aoi)
    {
        pathAoI.append(crd.value<QGeoCoordinate>());
    }

    if(pathAoI.empty())
    {
        return;
    }

//    PhotoUavModel uavModel(60, D2R(30));
    auto uavModel = CreatePhotoUavModelFromGui();
//    PhotoCameraModel photoCameraModel(0.02, 0.015, 0.0225);
    auto photoCameraModel = CreatePhotoCameraModelFromGui();
    UpdateCalculationParamsFromGui();
    AreaPhotoRegion region(pathAoI);
    m_apPhotoPlanner.reset(new AreaPhotoPlanner(uavModel, photoCameraModel, region));
    dynamic_cast<AreaPhotoPlanner *>(m_apPhotoPlanner.get())->Calculate(altitude(), longitOverlap(), transverseOverlap(), azimuth(),  1);

    UpdatePhotoPlannerDraw();
}

void PhotoPlan::UpdatePhotoPlannerDraw()
{
    m_sourceTrack = m_apPhotoPlanner->GetTrackPoints();
    m_photoCenters.clear();
    m_photoPrints.clear();
    for (auto photoPrint : m_apPhotoPlanner->GetPhotoPrints()) {
        m_photoPrints.append(photoPrint.GetBorder());
        m_photoCenters.append(photoPrint.GetCenter());
    }
    m_flightPoints = m_apPhotoPlanner->GetFlightPoints();
}

void PhotoPlan::saveFlightPlan(QVariant fileurl, int pointsInPacket)
{
    auto fileurlcvt = fileurl.value<QUrl>().toLocalFile();
    if (m_apPhotoPlanner) {
        m_apPhotoPlanner->SaveToXml(fileurlcvt, pointsInPacket);
    }
}

const char *PhotoPlanSerializationSignature = "PhotoPlannerTrack";
QVariantList PhotoPlan::loadAoi(QVariant fileurl)
{
    m_aoi.clear();
    auto fileurlcvt = fileurl.value<QUrl>().toLocalFile();
    {
        QFile file(fileurlcvt);
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_9);

        QString magicString;
        in >> magicString;
        if (magicString == PhotoPlanSerializationSignature) {
            quint32 version;
            in >> version;
            if (version == 1) {
                QString cameraModelName, uavModelName;
                in >> cameraModelName >> uavModelName;
                setCameraModelName(cameraModelName);
                setUavModelName(uavModelName);
                in >> m_missionType;
                in >> m_aoi;
            }
            return aoi();
        }
    }

    // Read outdated format
    QFile file(fileurlcvt);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> m_missionType;
    in >> m_aoi;
    return aoi();
}


void PhotoPlan::saveAoi(QVariant fileurl, QString poiType, QVariantList aoi)
{
    m_missionType = poiType;
    m_aoi.clear();
    foreach(QVariant crd, aoi)
    {
        m_aoi.append(crd.value<QGeoCoordinate>());
    }

    auto fileurlcvt = fileurl.value<QUrl>().toLocalFile();
    if(!fileurlcvt.endsWith(".aoi"))
    {
        fileurlcvt.append(".aoi");
    }

    QFile file(fileurlcvt);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_9);

    out<<QString("PhotoPlannerTrack");
    out<<quint32(1);

    out << m_cameraModelName;
    out << m_uavModelName;
    out << m_missionType;
    out << m_aoi;
}

QGeoCoordinate PhotoPlan::getFlightPointCoord(int index)
{
    return QGeoCoordinate(m_flightPoints.at(index).latitude(),m_flightPoints.at(index).longitude());
}
quint8 PhotoPlan::getFlightPointType(int index)
{
    return m_flightPoints.at(index).type();
}
quint32 PhotoPlan::getFlightPointCount()
{
    return m_flightPoints.count();
}
