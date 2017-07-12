#include <QtTest>

// add necessary includes here

#include "photogeocalcs.h"
#include "photoprintsgenerator.h"

#include <proj_api.h>

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

    void test_geo_calcs() {
//        using namespace aero_photo;
//        const double distance = 5000;
//        const double azimuth = 0;
//        RunStartPointsCalc calc(GeoPoint(0, 0), azimuth, distance, 2);
//        GeoPoints starts = { calc.Calculate(0), calc.Calculate(1) };
//        for (double offset = 0; offset < 2e7; offset += 1e6) {
//            qDebug("Checked offset %f %f", offset, starts[0].atDistanceAndAzimuth(offset, azimuth).distanceTo(starts[1].atDistanceAndAzimuth(offset, azimuth)));
//        }
    }

    void test_case1() {
        //    using namespace aero_photo;
        //    GeoPoint startPoint(47.2589912414551, 11.3327512741089);
        //    GeoPoints testAreaBorder;
        //    testAreaBorder << startPoint;
        //    testAreaBorder << testAreaBorder.back().atDistanceAndAzimuth(1100, 90);
        //    testAreaBorder << testAreaBorder.back().atDistanceAndAzimuth(220, 180);
        //    testAreaBorder << testAreaBorder.back().atDistanceAndAzimuth(1100, -90);
        //    testAreaBorder << testAreaBorder.back().atDistanceAndAzimuth(220, 0);
        //    QVERIFY (testAreaBorder.front().distanceTo(testAreaBorder.back()) < 1);
        //    TargetArea testArea(testAreaBorder);
        //    LinearPhotoPrintsGenerator generator(testArea);
        //    auto photoPrints = generator.GeneratePhotoPrints(200, 50);
        //    QCOMPARE(photoPrints.size(), 16);
    }

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
