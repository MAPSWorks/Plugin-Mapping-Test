#include "photoplan.h"
#include "PhotoPlanner/PhotoPrintsGenerator.h"


PhotoPlan::PhotoPlan()
{

}

GeoCoordinatesModel* PhotoPlan::AOIModel()
{
    return &m_AOIModel;
}

GeoCoordinatesModel* PhotoPlan::FlightModel()
{
    return &m_FlightModel;
}

GeoCoordinatesModel* PhotoPlan::PhotoPointsModel()
{
    return &m_PhotoPointsModel;
}

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void PhotoPlan::calcLinearPhotoPrints()
{
    using namespace aero_photo;

    auto linearTestFun = [](auto &&track) {
        LinearPhotoRegion region(track);
        LinearPhotoPrintsGenerator generator(region);
        auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(200, 90, 4);
        auto photoPrints = generator.GeneratePhotoPrints(photoPrintsCenters, 250, 120);
        return photoPrints;
    };
    auto photoPrints = linearTestFun(m_AOIModel.getGeoCoordinates());
    m_FlightModel.clear();
    for (auto photoPrint : photoPrints) {
        //        m_GeoCoordinates.append(photoPrint.GetBorder());
        m_FlightModel.addGeoCoordinate(photoPrint.GetCenter());
    }
}

void PhotoPlan::calcAreaPhotoPrints()
{
    using namespace aero_photo;

    auto areaTestFun = [](auto &&track) {
        AreaPhotoRegion testArea(track);
        AreaPhotoPrintsGenerator generator(testArea);
        auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(200, 90, 90);
//        auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(200, 90, generator.GetPreferredAzimuth());
        auto photoPrints = generator.GeneratePhotoPrints(photoPrintsCenters, 250, 120);
        return photoPrints;
    };
    auto photoPrints = areaTestFun(m_AOIModel.getGeoCoordinates());
    m_FlightModel.clear();
    for (auto photoPrint : photoPrints) {
        //        m_GeoCoordinates.append(photoPrint.GetBorder());
        m_FlightModel.addGeoCoordinate(photoPrint.GetCenter());
    }
}

