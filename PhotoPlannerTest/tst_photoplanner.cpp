#include <QtTest>

// add necessary includes here

#include "photogeocalcs.h"
#include "PhotoPrintsGenerator.h"

#include <proj_api.h>
#include <geodesic.h>

class PhotoPlanner : public QObject
{
    Q_OBJECT

public:
    PhotoPlanner();
    ~PhotoPlanner();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_basic_math() {
        QCOMPARE(sqrt(25), double(5));
    }

    void test_proj4() {
        projPJ pj_merc = pj_init_plus("+proj=merc +ellps=clrk66 +lat_ts=33");
        QVERIFY(pj_merc!=nullptr);
        projPJ pj_latlong = pj_init_plus("+proj=latlong +ellps=clrk66");
        QVERIFY(pj_latlong!=nullptr);
        double x = -16; x *= DEG_TO_RAD;
        double y = 20.25; y*= DEG_TO_RAD;
        auto errcode = pj_transform(pj_latlong, pj_merc, 1, 1, &x, &y, NULL );
        QCOMPARE (errcode, 0);
        // qDebug("x='%f' y='%f'", x, y);
        QCOMPARE (x, -1495284.211473);
        QCOMPARE (y, 1920596.789917);
        pj_free(pj_latlong);
        pj_free(pj_merc);
    }

    void test_GeoCalcsRoundUpPoints() {
        using namespace aero_photo;

        GeoCalc geoCalc;
        GeoPoint pnt1(1, 1);

        {
            GeoPoint pnt2 = geoCalc.AtDistanceAndAzimuth(pnt1, 900, 45);
            QCOMPARE (geoCalc.RoundUpPoints(pnt1, pnt2, 200), GeoDistance(1000));
        }

        {
            GeoPoint pnt2 = geoCalc.AtDistanceAndAzimuth(pnt1, 9999, 90);
            QCOMPARE (geoCalc.RoundUpPoints(pnt1, pnt2, 200), GeoDistance(10000));
        }

        {
            GeoPoint startPoint(47.2589912414551, 11.3327512741089);
            GeoPoint finishPoint = geoCalc.AtDistanceAndAzimuth(startPoint, 999, 90);
            QCOMPARE(geoCalc.RoundUpPoints(startPoint, finishPoint, 750.0), GeoDistance(1500));
        }


//        const double distance = 5000;
//        const double azimuth = 0;
//        RunStartPointsCalc calc(GeoPoint(0, 0), azimuth, distance, 2);
//        GeoPoints starts = { calc.Calculate(0), calc.Calculate(1) };
//        for (double offset = 0; offset < 2e7; offset += 1e6) {
//            qDebug("Checked offset %f %f", offset, starts[0].atDistanceAndAzimuth(offset, azimuth).distanceTo(starts[1].atDistanceAndAzimuth(offset, azimuth)));
//        }
    }


    void test_GeoCalcs() {
        using namespace aero_photo;


//        const double distance = 5000;
//        const double azimuth = 0;
//        RunStartPointsCalc calc(GeoPoint(0, 0), azimuth, distance, 2);
//        GeoPoints starts = { calc.Calculate(0), calc.Calculate(1) };
//        for (double offset = 0; offset < 2e7; offset += 1e6) {
//            qDebug("Checked offset %f %f", offset, starts[0].atDistanceAndAzimuth(offset, azimuth).distanceTo(starts[1].atDistanceAndAzimuth(offset, azimuth)));
//        }
    }

    void test_LinearPhotoPrintsGenerator_CalculateSequnce() {
        using namespace aero_photo;
        GeoPoints track4Points;
        track4Points << GeoPoint(1, 1) << GeoPoint(2, 2) << GeoPoint(3, 3) << GeoPoint(4, 4);
        LinearPhotoRegion region(track4Points);
        LinearPhotoPrintsGenerator generator(region);
        QCOMPARE (generator.CalculateRunSequnce(0, 0), 0);
        QCOMPARE (generator.CalculateRunSequnce(2, 0), 2);
        QCOMPARE (generator.CalculateRunSequnce(0, 1), 5);
        QCOMPARE (generator.CalculateRunSequnce(2, 1), 3);
        QCOMPARE (generator.CalculateRunSequnce(0, 2), 6);
        QCOMPARE (generator.CalculateRunSequnce(2, 2), 8);
        QCOMPARE (generator.CalculateRunSequnce(0, 3), 11);
        QCOMPARE (generator.CalculateRunSequnce(2, 3), 9);
    }

    void test_LinearPhotoPrintsGenerator() {
        using namespace aero_photo;
        GeoPoint startPoint(47.2589912414551, 11.3327512741089);
        GeoPoints track;
        {
            LinearPhotoRegion photoRegion(track);
            QVERIFY_EXCEPTION_THROWN(LinearPhotoPrintsGenerator generator(photoRegion), std::logic_error);
        }
        track << startPoint;
        {
            LinearPhotoRegion photoRegion(track);
            QVERIFY_EXCEPTION_THROWN(LinearPhotoPrintsGenerator generator(photoRegion), std::logic_error);
        }
        track << track.back().atDistanceAndAzimuth(999.5, 90);
        {
            LinearPhotoRegion photoRegion(track);
            LinearPhotoPrintsGenerator generator(photoRegion);
            {
                auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters( 100, 100, 1);
                QCOMPARE(photoPrintsCenters.size(), 1);
                QCOMPARE(photoPrintsCenters[0].size(), 11);
            }
            {
                auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters( 100, 1000, 2);
                QCOMPARE(photoPrintsCenters.size(), 2);
                QCOMPARE(photoPrintsCenters[0].size(), 11);
                QCOMPARE(photoPrintsCenters[1].size(), 11);
            }
            {
                auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters( 100, 500, 9);
                QCOMPARE(photoPrintsCenters.size(), 9);
            }
            {
                auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters( 50, 1000, 1);
                QCOMPARE(photoPrintsCenters.size(), 1);
                QCOMPARE(photoPrintsCenters[0].size(), 21);
            }
            {
                auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters( 33, 1000, 1);
                QCOMPARE(photoPrintsCenters.size(), 1);
                QCOMPARE(photoPrintsCenters[0].size(), 32);
            }
            {
                auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters( 750, 1000, 1);
                QCOMPARE(photoPrintsCenters.size(), 1);
                QCOMPARE(photoPrintsCenters[0].size(), 3);
            }
        }

        track << track.back().atDistanceAndAzimuth(220, 180);
        track << track.back().atDistanceAndAzimuth(1100, -90);
    }

//    void test_AreaPhotoPrintsGenerator() {
//        using namespace aero_photo;
//        GeoPoint startPoint(47.2589912414551, 11.3327512741089);
//        GeoPoints testAreaBorder;
//        testAreaBorder << startPoint;
//        testAreaBorder << testAreaBorder.back().atDistanceAndAzimuth(1100, 90);
//        testAreaBorder << testAreaBorder.back().atDistanceAndAzimuth(220, 180);
//        testAreaBorder << testAreaBorder.back().atDistanceAndAzimuth(1100, -90);
//        testAreaBorder << testAreaBorder.back().atDistanceAndAzimuth(220, 0);
//        QVERIFY (testAreaBorder.front().distanceTo(testAreaBorder.back()) < 1);
//        AreaPhotoRegion testArea(testAreaBorder);
//        AreaPhotoPrintsGenerator generator(testArea);
//        auto photoPrints = generator.GeneratePhotoPrints(45, 100, 50, 125, 75);
//        //QCOMPARE(photoPrints.size(), 16);
//    }


};

PhotoPlanner::PhotoPlanner()
{
}

PhotoPlanner::~PhotoPlanner()
{

}

void PhotoPlanner::initTestCase()
{

}

void PhotoPlanner::cleanupTestCase()
{

}

QTEST_APPLESS_MAIN(PhotoPlanner)

#include "tst_photoplanner.moc"
