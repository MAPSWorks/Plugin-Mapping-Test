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
