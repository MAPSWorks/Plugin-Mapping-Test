#include "photoplannerwindow.h"
#include <QQmlContext>
#include <QDebug>
#include <QTranslator>
#include <QApplication>

PhotoPlannerWindow::PhotoPlannerWindow()
{
    QTranslator* pTranslator = new QTranslator;
    pTranslator->load("PhotoPlanner_ru.qm", QApplication::applicationDirPath()+"/translations/gcu/");
    QApplication::installTranslator(pTranslator);

    qmlRegisterType<PhotoPlan>("PhotoPlanner", 1, 0, "PhotoPlanner");
    rootContext()->setContextProperty("photoPlanner", &photoPlan);

    camerasModel.AddCamera(new QCameraData("Sony A6000 [Sel20F28]", 20, 15, 22.5, 3648, 5472, &camerasModel));
    camerasModel.AddCamera(new QCameraData("Sony A6000 [Sel35F18]", 35, 15, 22.5, 3648, 5472, &camerasModel));
    camerasModel.AddCamera(new QCameraData("Sony S600 35", 35, 15, 22.5, 2112, 2816, &camerasModel));
    camerasModel.AddCamera(new QCameraData("Sony S600 50", 50, 15, 22.5, 2112, 2816, &camerasModel));

    qmlRegisterType<QCameraData>("QCameraData", 1, 0, "QCameraData");
    rootContext()->setContextProperty("camerasModel", &camerasModel);

    setWindowTitle(tr("Photo Planner"));
    setSource(QUrl("qrc:/qml/PhotoPlanner.qml"));

    setResizeMode(SizeRootObjectToView);
}


bool PhotoPlannerWindow::aboutToQuit()
{
    return true;
}
