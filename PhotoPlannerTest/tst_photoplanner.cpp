#include <QtTest>

// add necessary includes here

#include "PhotoGeoCalcs.h"
#include "PhotoPrintsGenerators.h"
#include "PhotoCameraModel.h"
#include "AreaPhotoPlanner.h"
#include "LinearPhotoPlanner.h"

class Tst_PhotoPlanner : public QObject
{
    Q_OBJECT

public:
    Tst_PhotoPlanner();
    ~Tst_PhotoPlanner();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_basic_math() {
        QCOMPARE(sqrt(25), double(5));
    }

    void test_Array2D() {
        using namespace aero_photo;

        Array2D<uint8_t> array2D(5, 4);
        for(size_t iRow = 0; iRow < array2D.Rows(); iRow++)
            for(size_t iCol = 0; iCol < array2D.Cols(); iCol++)
                QCOMPARE (array2D.Item(iRow, iCol), (uint8_t)0);

        array2D.Item(0,0)=1;
        array2D.Item(0,2)=2;
        array2D.Item(1,1)=3;
        array2D.Item(4,3)=0;
        QCOMPARE (array2D.Item(0,0), (uint8_t)1);
        QCOMPARE (array2D.Item(0,2), (uint8_t)2);
        QCOMPARE (array2D.Item(1,1), (uint8_t)3);
        QCOMPARE (array2D.Item(4,3), (uint8_t)0);
    }

    void test_Azimuth() {
        using namespace aero_photo;
        {
            Azimuth az;
            QCOMPARE ((qreal)az, 0.0);
            Azimuth azM(Azimuth(45));
            QCOMPARE ((qreal)azM, 45.0);
        }
        {
            Azimuth az(90);
            Azimuth az2(az);
            Azimuth az3 = az2;
            Azimuth az4 = -90.0;
            QCOMPARE ((qreal)az, 90.0);
            QCOMPARE ((qreal)az2, 90.0);
            QCOMPARE ((qreal)az3, 90.0);
            QCOMPARE ((qreal)az4, -90.0);

            az = -90;
            QCOMPARE ((qreal)az, -90.0);
        }
        {
            Azimuth az(225);
            Azimuth az2(-225);
            Azimuth az3(45);
            Azimuth az4(-45);
            QCOMPARE ((qreal)az, -135.0);
            QCOMPARE ((qreal)az2, 135.0);
            QCOMPARE ((qreal)az3, 45.0);
            QCOMPARE ((qreal)az4, -45.0);
        }
        {
            QCOMPARE (Azimuth(90) == Azimuth(-90), false);
            Azimuth az(180);
            Azimuth az2(-180);
            QCOMPARE ((qreal)az, -180.0);
            QCOMPARE ((qreal)az2, -180.0);
            QCOMPARE (az == az2, true);
        }
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

    void test_PhotoCameraModel() {
        aero_photo::PhotoCameraModel sonyA6000(0.02, 0.015, 0.0225);
        double Lx = 0, Ly = 0;
        sonyA6000.CalcLxLy(100, Lx, Ly);
        QCOMPARE(Lx, 75.0);
        QCOMPARE(Ly, 112.5);
        sonyA6000.CalcLxLy(500, Lx, Ly);
        QCOMPARE(Lx, 375.0);
        QCOMPARE(Ly, 562.5);

        double Bx = 0, By = 0;
        sonyA6000.CalcBxBy(100, 80, 60, Bx, By);
        QCOMPARE(Bx, 15.0);
        QCOMPARE(By, 45.0);
        sonyA6000.CalcBxBy(500, 80, 60, Bx, By);
        QCOMPARE(Bx, 75.0);
        QCOMPARE(By, 225.0);
    }

    void test_GeoCalcs() {
        QCOMPARE( aero_photo::D2R(0), 0.0);
        QCOMPARE( aero_photo::D2R(180), M_PI);
        QCOMPARE( aero_photo::R2D(0), 0.0);
        QCOMPARE( aero_photo::R2D(M_PI), 180.0);

//        using namespace aero_photo;


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
                auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(500, 100, 100, 1);
                QCOMPARE(photoPrintsCenters.size(), 1);
                QCOMPARE(photoPrintsCenters[0].size(), 11);
            }
            {
                auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(500, 100, 1000, 2);
                QCOMPARE(photoPrintsCenters.size(), 2);
                QCOMPARE(photoPrintsCenters[0].size(), 11);
                QCOMPARE(photoPrintsCenters[1].size(), 11);
            }
            {
                auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(500,  100, 500, 9);
                QCOMPARE(photoPrintsCenters.size(), 9);
            }
            {
                auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(500,  50, 1000, 1);
                QCOMPARE(photoPrintsCenters.size(), 1);
                QCOMPARE(photoPrintsCenters[0].size(), 21);
            }
            {
                auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(1000, 33, 1000, 1);
                QCOMPARE(photoPrintsCenters.size(), 1);
                QCOMPARE(photoPrintsCenters[0].size(), 32);
            }
            {
                auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters(1000, 750, 1000, 1);
                QCOMPARE(photoPrintsCenters.size(), 1);
                QCOMPARE(photoPrintsCenters[0].size(), 3);
            }
        }

        track << track.back().atDistanceAndAzimuth(220, 180);
        track << track.back().atDistanceAndAzimuth(1100, -90);
    }

    void test_AreaPhotoPrintsGenerator() {
        using namespace aero_photo;

        GeoPoint startPoint(47.2589912414551, 11.3327512741089);
        GeoPoints border;

        {
            AreaPhotoRegion photoRegion(border);
            QVERIFY_EXCEPTION_THROWN(AreaPhotoPrintsGenerator generator(photoRegion), std::logic_error);
        }
        border << startPoint;
        {
            AreaPhotoRegion photoRegion(border);
            QVERIFY_EXCEPTION_THROWN(AreaPhotoPrintsGenerator generator(photoRegion), std::logic_error);
        }
        border << border.back().atDistanceAndAzimuth(1100, 90);
        {
            AreaPhotoRegion photoRegion(border);
            QVERIFY_EXCEPTION_THROWN(AreaPhotoPrintsGenerator generator(photoRegion), std::logic_error);
        }
        border << border.back().atDistanceAndAzimuth(220, 180);
        {
            AreaPhotoRegion photoRegion(border);
            AreaPhotoPrintsGenerator generator(photoRegion);
            {
                auto photoPrintsCenters = generator.GeneratePhotoPrintsCenters( 200, 90, 90);
                QCOMPARE(photoPrintsCenters.size(), 13);
                QCOMPARE(photoPrintsCenters[0].size(), 0);
                QCOMPARE(photoPrintsCenters[1].size(), 0);
                QCOMPARE(photoPrintsCenters[2].size(), 0);
                QCOMPARE(photoPrintsCenters[3].size(), 0);
                QCOMPARE(photoPrintsCenters[4].size(), 0);
                QCOMPARE(photoPrintsCenters[5].size(), 5);
                QCOMPARE(photoPrintsCenters[6].size(), 2);
                QCOMPARE(photoPrintsCenters[7].size(), 0);
                QCOMPARE(photoPrintsCenters[8].size(), 0);
                QCOMPARE(photoPrintsCenters[9].size(), 0);
                QCOMPARE(photoPrintsCenters[10].size(), 0);
                QCOMPARE(photoPrintsCenters[11].size(), 0);
                QCOMPARE(photoPrintsCenters[12].size(), 0);
            }
        }
//        border << border.back().atDistanceAndAzimuth(1100, -90);
//        border << border.back().atDistanceAndAzimuth(220, 0);
    }

    void test_PhotoPlannerCreate() {
        using namespace aero_photo;

        PhotoUavModel fakeUav(10, D2R(45));
        PhotoCameraModel sonyA6000(0.02, 0.015, 0.0225);
        GeoPoints track;
        GeoPoint startPoint(47.2589912414551, 11.3327512741089);
        track << startPoint;
        track << track.back().atDistanceAndAzimuth(1000, 90);
        {
            LinearPhotoRegion photoRegion(track);
            LinearPhotoPlanner planner(fakeUav, sonyA6000, photoRegion);
        }
        track << track.back().atDistanceAndAzimuth(1000, 180);
        {
            AreaPhotoRegion photoRegion(track);
            AreaPhotoPlanner planner(fakeUav, sonyA6000, photoRegion);
        }
    }

    void test_PhotoUavModelCalcs() {
        using namespace aero_photo;
        PhotoUavModel uavModel(10, D2R(45));
        QCOMPARE(uavModel.GetManeuverR(), 100.0 / 9.81);
    }

    void test_ManeuverTrackAlignment() {
        using namespace aero_photo;

        PhotoUavModel uavModel(100, D2R(45));
        auto R = uavModel.GetManeuverR();

        GeoPoint pnt1(47.2589912414551, 11.3327512741089);
        GeoPoint pnt2 = pnt1.atDistanceAndAzimuth(3 * R, 45);
        ManeuverTrackAlignment mnvAligment(pnt1, 0, pnt2, 90);
        auto turnPoints = mnvAligment.Calculate(uavModel);
        QCOMPARE(turnPoints.size(), 2);
    }


};

Tst_PhotoPlanner::Tst_PhotoPlanner()
{
}

Tst_PhotoPlanner::~Tst_PhotoPlanner()
{

}

void Tst_PhotoPlanner::initTestCase()
{

}

void Tst_PhotoPlanner::cleanupTestCase()
{

}

QTEST_APPLESS_MAIN(Tst_PhotoPlanner)

#include "tst_photoplanner.moc"
