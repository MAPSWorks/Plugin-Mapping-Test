#include "photoplannerwindow.h"
#include <QQmlContext>
#include <QDebug>

PhotoPlannerWindow::PhotoPlannerWindow()
{



    setWindowTitle(tr("Photo Planner"));
    setSource(QUrl("qrc:/qml/PhotoPlanner.qml"));

    qmlRegisterType<PhotoPlan>("PhotoPlan", 1, 0, "PhotoPlan");

    rootContext()->setContextProperty("photoPlanner", &photoPlan);

    setResizeMode(SizeRootObjectToView);
}


bool PhotoPlannerWindow::aboutToQuit()
{
    return true;
}

QVariantList PhotoPlannerWindow::aoi() const
{
//   return QList<QGeoCoordinate> m_aoi.toList();
}

void PhotoPlannerWindow::setAoi(const QVariantList &aoi)
{
    //m_aoi = aoi;
}

void PhotoPlannerWindow::calculateAoI(QVariantList aoi)
{
    QVector<QGeoCoordinate> pathAoI;

    foreach(QVariant crd, aoi)
    {
        pathAoI.append(crd.value<QGeoCoordinate>());
    }

    double k= pathAoI[0].latitude();
    k=0;
}
