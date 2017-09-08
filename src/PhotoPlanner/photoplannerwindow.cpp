#include "photoplannerwindow.h"
#include <QQmlContext>
#include <QDebug>

PhotoPlannerWindow::PhotoPlannerWindow()
{
    setWindowTitle(tr("Photo Planner"));
    setSource(QUrl("qrc:/qml/PhotoPlanner.qml"));

    qmlRegisterType<PhotoPlan>("PhotoPlanner", 1, 0, "PhotoPlanner");

    rootContext()->setContextProperty("photoPlanner", &photoPlan);

    setResizeMode(SizeRootObjectToView);
}


bool PhotoPlannerWindow::aboutToQuit()
{
    return true;
}
