#include <QtTest>

// add necessary includes here

#include "photoprintsgenerator.h"

class PhotoPlanner : public QObject
{
    Q_OBJECT

public:
    PhotoPlanner();
    ~PhotoPlanner();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

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

void PhotoPlanner::test_case1()
{
    using namespace aero_photo;
    GeoPoint startPoint(47.2589912414551, 11.3327512741089);
    GeoPoints testAreaBorder;
    testAreaBorder << startPoint;
    testAreaBorder << testAreaBorder.back().atDistanceAndAzimuth(1100, 90);
    testAreaBorder << testAreaBorder.back().atDistanceAndAzimuth(220, 180);
    testAreaBorder << testAreaBorder.back().atDistanceAndAzimuth(1100, -90);
    testAreaBorder << testAreaBorder.back().atDistanceAndAzimuth(220, 0);
    QVERIFY (testAreaBorder.front().distanceTo(testAreaBorder.back()) < 1);
    TargetArea testArea(testAreaBorder);
    LinePhotoPrintsGenerator generator(testArea);
    auto photoPrints = generator.GeneratePhotoPrints(200, 50);
    QCOMPARE(photoPrints.size(), 16);
}

QTEST_APPLESS_MAIN(PhotoPlanner)

#include "tst_photoplanner.moc"
