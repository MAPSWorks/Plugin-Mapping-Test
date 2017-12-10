#ifndef PHOTOPLANNERWINDOW_H
#define PHOTOPLANNERWINDOW_H

#include <QQuickWidget>
#include <QGeoCoordinate>
#include "photoplan.h"
#include "camerasmodel.h"
#include "uavsmodel.h"

class PhotoPlannerWindow : public QQuickWidget
{
    Q_OBJECT
public:
    PhotoPlannerWindow();
    ~PhotoPlannerWindow();

public slots:
    bool aboutToQuit();

private:
    void InitConfiguration();
    bool ReadConfiguration();
    bool WriteConfiguration();

private:
    QCamerasModel camerasModel;
    QUavsModel uavsModel;
    PhotoPlan photoPlan;
};

#endif // PHOTOPLANNERWINDOW_H
