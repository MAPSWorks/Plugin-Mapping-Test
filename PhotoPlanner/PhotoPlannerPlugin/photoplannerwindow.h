#ifndef PHOTOPLANNERWINDOW_H
#define PHOTOPLANNERWINDOW_H

#include <QQuickWidget>
#include <QGeoCoordinate>
#include "photoplan.h"
#include "camerasmodel.h"
#include "uavsmodel.h"

class PhotoPlannerOptions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isPhotoPrintsVisible READ isPhotoPrintsVisible WRITE setIsPhotoPrintsVisible NOTIFY isPhotoPrintsVisibleChanged)
    Q_PROPERTY(quint32 mapTypeIndex READ mapTypeIndex WRITE setMapTypeIndex NOTIFY mapTypeIndexChanged)

public:
    explicit PhotoPlannerOptions(QObject *parent = nullptr) : QObject(parent)  { }

    quint32 mapTypeIndex() const { return mapTypeIndex_; }
    void setMapTypeIndex(const quint32 &value) {
        if(value != mapTypeIndex_)
        {
            mapTypeIndex_ = value;
            emit mapTypeIndexChanged();
        }
    }

    bool isPhotoPrintsVisible() const { return isPhotoPrintsVisible_; }
    void setIsPhotoPrintsVisible(const bool value) {
        if (value != isPhotoPrintsVisible_) {
            isPhotoPrintsVisible_ = value;
            emit isPhotoPrintsVisibleChanged();
        }
    }

signals:
    void mapTypeIndexChanged();
    void isPhotoPrintsVisibleChanged();

private:
    quint32 mapTypeIndex_ = 0;
    bool isPhotoPrintsVisible_ = false;
};


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
    PhotoPlannerOptions photoPlannerOptions;
};

#endif // PHOTOPLANNERWINDOW_H
