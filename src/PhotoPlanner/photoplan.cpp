#include "photoplan.h"
#include "PhotoPlanner/PhotoPrintsGenerator.h"

#include <QVariant>


PhotoPlan::PhotoPlan(QObject *parent) : QObject(parent)
{
    m_cameraModel           = "Sony S600";
    m_focusRange            = 35;
    m_longitOverlap         = 80;
    m_transverseOverlap     = 80;
    m_azimuth               = 0;
    m_altitude              = 100;
    m_gsd                   = 150;
    m_speed                 = 30;
    m_width                 = 100;
    m_maxRoll               = 30;

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

quint32 PhotoPlan::gsd() const
{
    return m_gsd;
}

void PhotoPlan::setGsd(const quint32 &gsd)
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

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void PhotoPlan::calcLinearPhotoPrints(QVariantList aoi)
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

    auto linearTestFun = [](auto &&track) {
        LinearPhotoRegion region(track);
        LinearPhotoPrintsGenerator generator(region);
        auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(200, 90, 4);
        auto photoPrints = generator.GeneratePhotoPrints(photoPrintsCenters, 250, 120);
        return photoPrints;
    };



    auto photoPrints = linearTestFun(pathAoI);

    m_sourceTrack.clear();
    m_photoCenters.clear();
    m_photoPrints.clear();

    for (auto photoPrint : photoPrints) {
                m_photoPrints.append(photoPrint.GetBorder());
                m_photoCenters.append(photoPrint.GetCenter());
                m_sourceTrack.append(photoPrint.GetCenter());

    }
//        m_FlightModel.addGeoCoordinate(photoPrint.GetCenter());
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

    auto areaTestFun = [](auto &&track) {
        AreaPhotoRegion testArea(track);
        AreaPhotoPrintsGenerator generator(testArea);
        auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(200, 90, 90);
//        auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(200, 90, generator.GetPreferredAzimuth());
        auto photoPrints = generator.GeneratePhotoPrints(photoPrintsCenters, 250, 120);
        return photoPrints;
    };

    auto photoPrints = areaTestFun(pathAoI);

    m_sourceTrack.clear();
    m_photoCenters.clear();
    m_photoPrints.clear();

    for (auto photoPrint : photoPrints) {
                m_photoPrints.append(photoPrint.GetBorder());
//        m_FlightModel.addGeoCoordinate(photoPrint.GetCenter());
      m_photoCenters.append(photoPrint.GetCenter());

      m_sourceTrack.append(photoPrint.GetCenter());
    }
 //   emit trackChanged();

}

