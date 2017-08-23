#include "photoplannerwindow.h"

PhotoPlannerWindow::PhotoPlannerWindow()
{
    setWindowTitle(tr("Photo Planner"));
    setSource(QUrl("qrc:/qml/PhotoPlanner.qml"));
    setResizeMode(SizeRootObjectToView);
}


bool PhotoPlannerWindow::aboutToQuit()
{
    return true;
}
