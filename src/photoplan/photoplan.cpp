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

void PhotoPlan::recalc()
{
    using namespace aero_photo;

    auto getDefaultTrack = []() {
        GeoPoint startPoint(47.2589912414551, 11.3247512741089);
        GeoPoints defaultTrack;
        defaultTrack << startPoint;
        defaultTrack << defaultTrack.back().atDistanceAndAzimuth(1500, 90);
        defaultTrack << defaultTrack.back().atDistanceAndAzimuth(750, 180);
//        defaultTrack << defaultTrack.back().atDistanceAndAzimuth(1100, -90);
//        defaultTrack << defaultTrack.back().atDistanceAndAzimuth(220, 0);
        return defaultTrack;
    };

    auto areaTestFun = [](auto &&track) {
        AreaPhotoRegion testArea(track);
        AreaPhotoPrintsGenerator generator(testArea);
        auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(200, 90, 90);
//        auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(200, 90, generator.GetPreferredAzimuth());
        auto photoPrints = generator.GeneratePhotoPrints(photoPrintsCenters, 250, 120);
        return photoPrints;
    };


    auto linearTestFun = [](auto &&track) {
        LinearPhotoRegion region(track);
        LinearPhotoPrintsGenerator generator(region);
        auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(200, 90, 4);
        auto photoPrints = generator.GeneratePhotoPrints(photoPrintsCenters, 250, 120);
        return photoPrints;
    };

    static int counter = 0;
//    m_GeoCoordinates = getDefaultTrack();

   // QVector<QGeoCoordinate> m_GeoCoordinates1;


    if (counter++ % 2 == 0) {
        auto photoPrints = areaTestFun(m_AOIModel.getGeoCoordinates());
        for (auto photoPrint : photoPrints) {
            //        m_GeoCoordinates.append(photoPrint.GetBorder());
            m_FlightModel.addGeoCoordinate(photoPrint.GetCenter());
        }
    }
}
