#ifndef PHOTOPLANNERWINDOW_H
#define PHOTOPLANNERWINDOW_H

#include <QQuickWidget>

class PhotoPlannerWindow : public QQuickWidget
{
    Q_OBJECT
public:
    PhotoPlannerWindow();
signals:

public slots:
    bool aboutToQuit();
};

#endif // PHOTOPLANNERWINDOW_H
