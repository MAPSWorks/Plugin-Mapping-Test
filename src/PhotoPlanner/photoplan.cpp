#include "photoplan.h"

#include <QDataStream>
#include <QFile>
#include <QUrl>
#include <QVariant>
#include <QtDebug>


PhotoPlan::PhotoPlan(QObject *parent) : QObject(parent)
{
    m_cameraModel           = "Sony S600";
    m_focusRange            = 35;
    m_longitOverlap         = 20;
    m_transverseOverlap     = 20;
    m_azimuth               = 0;
    m_altitude              = 100;
    m_gsd                   = 150;
    m_speed                 = 40;
    m_width                 = 100;
    m_maxRoll               = 30;
}

qreal PhotoPlan::calcPhotoParamGsdOnAltitude() {
    auto photoModel = CreatePhotoCameraModelFromGui();
    return photoModel.CalcGsd(altitude());
}

qreal PhotoPlan::calcPhotoParamAltitudeOnGsd() {
    auto photoModel = CreatePhotoCameraModelFromGui();
    return photoModel.CalcH(gsd());
}

QString PhotoPlan::cameraModel() const
{
    return m_cameraModel;
}

void PhotoPlan::setCameraModel(const QString &cameraModel)
{
    if(m_cameraModel != cameraModel)
    {
        m_cameraModel = cameraModel;
        emit cameraModelChanged();
    }
}

quint32 PhotoPlan::focusRange() const
{
    return m_focusRange;
}

void PhotoPlan::setFocusRange(const quint32 &focusRange)
{
    if(m_focusRange != focusRange)
    {
        m_focusRange = focusRange;
        emit focusRangeChanged();
    }
}

qreal PhotoPlan::cameraLx() const {
    return m_cameraLx;
}
void PhotoPlan::setCameraLx(const qreal &value){
    if(m_cameraLx != value)
    {
        m_cameraLx = value;
        emit cameraLxChanged();
    }
}

qreal PhotoPlan::cameraLy() const {
    return m_cameraLy;
}
void PhotoPlan::setCameraLy(const qreal &value) {
    if(m_cameraLy != value)
    {
        m_cameraLy = value;
        emit cameraLyChanged();
    }
}

qreal PhotoPlan::cameraAx() const {
    return m_cameraAx;
}
void PhotoPlan::setCameraAx(const qreal &value){
    if(m_cameraAx != value)
    {
        m_cameraAx = value;
        emit cameraAxChanged();
    }
}

qreal PhotoPlan::cameraAy() const {
    return m_cameraAy;
}
void PhotoPlan::setCameraAy(const qreal &value) {
    if(m_cameraAy != value)
    {
        m_cameraAy = value;
        emit cameraAyChanged();
    }
}

quint32 PhotoPlan::longitOverlap() const
{
    return m_longitOverlap;
}

void PhotoPlan::setLongitOverlap(const quint32 &longitOverlap)
{
    if(m_longitOverlap != longitOverlap)
    {
        m_longitOverlap = longitOverlap;
        emit longitOverlapChanged();
    }
}

quint32 PhotoPlan::transverseOverlap() const
{
    return m_transverseOverlap;
}

void PhotoPlan::setTransverseOverlap(const quint32 &transverseOverlap)
{
    if(m_transverseOverlap != transverseOverlap)
    {
        m_transverseOverlap = transverseOverlap;
        emit transverseOverlapChanged();
    }
}

quint32 PhotoPlan::azimuth() const
{
    return m_azimuth;
}

void PhotoPlan::setAzimuth(const quint32 &azimuth)
{
    if(m_azimuth != azimuth)
    {
        m_azimuth = azimuth;
        emit azimuthChanged();
    }
}

quint32 PhotoPlan::altitude() const
{
    return m_altitude;
}

void PhotoPlan::setAltitude(const quint32 &altitude)
{
    if(m_altitude != altitude)
    {
        m_altitude = altitude;
        emit altitudeChanged();
    }
}

qreal PhotoPlan::gsd() const
{
    return m_gsd;
}

void PhotoPlan::setGsd(const qreal &gsd)
{
    if(m_gsd != gsd)
    {
        m_gsd = gsd;
        emit gsdChanged();
    }
}

quint32 PhotoPlan::speed() const
{
    return m_speed;
}

void PhotoPlan::setSpeed(const quint32 &speed)
{
    if(m_speed != speed)
    {
        m_speed = speed;
        emit speedChanged();
    }
}

quint32 PhotoPlan::width() const
{
    return m_width;
}

void PhotoPlan::setWidth(const quint32 &width)
{
    if(m_width != width)
    {
        m_width = width;
        emit widthChanged();
    }
}

quint32 PhotoPlan::maxRoll() const
{
    return m_maxRoll;
}

void PhotoPlan::setMaxRoll(const quint32 &maxRoll)
{
    if(m_maxRoll != maxRoll)
    {
        m_maxRoll = maxRoll;
        emit maxRollChanged();
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

#undef D2R
#undef R2D

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

#include <algorithm>
#include <iostream>
#include <vector>

#include "PhotoPlannerMath/LinearPhotoPlanner.h"
#include "PhotoPlannerMath/AreaPhotoPlanner.h"


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

//    PhotoUavModel uavModel(60, D2R(30));
    auto uavModel = CreatePhotoUavModelFromGui();
//    PhotoCameraModel photoCameraModel(0.02, 0.015, 0.0225);
    auto photoCameraModel = CreatePhotoCameraModelFromGui();
    LinearPhotoRegion region(pathAoI);
    m_apPhotoPlanner.reset(new LinearPhotoPlanner(uavModel, photoCameraModel, region));
    dynamic_cast<LinearPhotoPlanner *>(m_apPhotoPlanner.get())->Calculate(altitude(), longitOverlap(), transverseOverlap(), width());

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

void PhotoPlan::saveFlightPlan(QVariant fileurl)
{
    auto fileurlcvt = fileurl.value<QUrl>().toLocalFile();
    if (m_apPhotoPlanner) {
        QFile xmlFile(fileurlcvt);
        if (xmlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            m_apPhotoPlanner->SaveToXml(&xmlFile);
        }
    }
}

QVariantList PhotoPlan::loadAoi(QVariant fileurl)
{
    auto fileurlcvt = fileurl.value<QUrl>().toLocalFile();
    QFile file(fileurlcvt);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> m_missionType;;
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
