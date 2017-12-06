#ifndef PHOTOPLANNERWINDOW_H
#define PHOTOPLANNERWINDOW_H

#include <QQuickWidget>
#include <QGeoCoordinate>
#include "photoplan.h"
#include "camerasmodel.h"

class PhotoPlannerWindow : public QQuickWidget
{
    Q_OBJECT
public:
    PhotoPlannerWindow();

public slots:
    bool aboutToQuit();

private:
    QCamerasModel camerasModel;
    PhotoPlan photoPlan;
};

#endif // PHOTOPLANNERWINDOW_H
