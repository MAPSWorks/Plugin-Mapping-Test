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

    if (!ReadConfiguration()) {
        InitConfiguration();
    }

    qmlRegisterType<PhotoPlan>("PhotoPlanner", 1, 0, "PhotoPlanner");
    rootContext()->setContextProperty("photoPlanner", &photoPlan);

    qmlRegisterType<QCameraData>("QCameraData", 1, 0, "QCameraData");
    rootContext()->setContextProperty("camerasModel", &camerasModel);
    qmlRegisterType<QUavData>("QUavData", 1, 0, "QUavData");
    rootContext()->setContextProperty("uavsModel", &uavsModel);

    setWindowTitle(tr("Photo Planner"));
    setSource(QUrl("qrc:/qml/PhotoPlanner.qml"));

    setResizeMode(SizeRootObjectToView);
}

PhotoPlannerWindow::~PhotoPlannerWindow()
{
    WriteConfiguration();
}


bool PhotoPlannerWindow::aboutToQuit()
{
    return true;
}

void PhotoPlannerWindow::InitConfiguration() {
    camerasModel.AddItem(new QCameraData("Sony A6000 [Sel20F28]", 20, 15, 22.5, 3648, 5472, &camerasModel));
    camerasModel.AddItem(new QCameraData("Sony A6000 [Sel35F18]", 35, 15, 22.5, 3648, 5472, &camerasModel));
    camerasModel.AddItem(new QCameraData("Sony S600 35", 35, 15, 22.5, 2112, 2816, &camerasModel));
    camerasModel.AddItem(new QCameraData("Sony S600 50", 50, 15, 22.5, 2112, 2816, &camerasModel));
    camerasModel.AddItem(new QCameraData("User camera 1", 50, 15, 22.5, 2112, 2816, &camerasModel));
    camerasModel.AddItem(new QCameraData("User camera 2", 50, 15, 22.5, 2112, 2816, &camerasModel));
    camerasModel.AddItem(new QCameraData("User camera 3", 50, 15, 22.5, 2112, 2816, &camerasModel));

    uavsModel.AddItem(new QUavData("Plane 1", 60, 15, 30, 25, &uavsModel));
    uavsModel.AddItem(new QUavData("Plane 2", 120, 20, 30, 50, &uavsModel));
    uavsModel.AddItem(new QUavData("Plane 3", 120, 20, 40, 60, &uavsModel));
    uavsModel.AddItem(new QUavData("Quadro 1", 40, 10, 90, 5, &uavsModel));
    uavsModel.AddItem(new QUavData("Quadro 2", 60, 30, 90, 15, &uavsModel));
    uavsModel.AddItem(new QUavData("Quadro 3", 60, 30, 80, 15, &uavsModel));
}

bool PhotoPlannerWindow::ReadConfiguration() {
    QFile file("photoplanner.cnf");
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QDataStream datastream(&file);
    datastream.setVersion(QDataStream::Qt_5_9);
    QString magicString;
    datastream >> magicString;
    if (magicString != "PhotoPlannerConfiguration")
        return false;
    quint32 version;
    datastream >> version;
    if (version != 1)
        return false;

    datastream >> camerasModel;
    datastream >> uavsModel;

    return true;
}

bool PhotoPlannerWindow::WriteConfiguration() {
    QFile file("photoplanner.cnf");
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QDataStream datastream(&file);
    datastream.setVersion(QDataStream::Qt_5_9);
    datastream << QString("PhotoPlannerConfiguration") << quint32(1);

    datastream << camerasModel;
    datastream << uavsModel;

    return true;
}
