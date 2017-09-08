#ifndef PHOTOPLANNERWINDOW_H
#define PHOTOPLANNERWINDOW_H

#include <QQuickWidget>
#include <QGeoCoordinate>
#include "photoplan.h"

class PhotoPlannerWindow : public QQuickWidget
{
    Q_OBJECT
public:
    PhotoPlannerWindow();

public slots:
    bool aboutToQuit();

private:

    PhotoPlan photoPlan;
};

#endif // PHOTOPLANNERWINDOW_H
