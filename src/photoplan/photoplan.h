#ifndef PHOTOPLAN_H
#define PHOTOPLAN_H

#include <QObject>
#include "geocoordinatesmodel.h"

class PhotoPlan : public QObject
{
    Q_OBJECT
    Q_PROPERTY(GeoCoordinatesModel* AOIModel            READ AOIModel)
    Q_PROPERTY(GeoCoordinatesModel* FlightModel         READ FlightModel)
    Q_PROPERTY(GeoCoordinatesModel* PhotoPointsModel    READ PhotoPointsModel)

public:
    PhotoPlan();
    GeoCoordinatesModel* AOIModel();
    GeoCoordinatesModel* FlightModel();
    GeoCoordinatesModel* PhotoPointsModel();


    Q_INVOKABLE void calcLinearPhotoPrints();
    Q_INVOKABLE void calcAreaPhotoPrints();

private:
    GeoCoordinatesModel m_AOIModel;
    GeoCoordinatesModel m_FlightModel;
    GeoCoordinatesModel m_PhotoPointsModel;
};

#endif // PHOTOPLAN_H
