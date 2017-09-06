#ifndef PHOTOPLANNERWINDOW_H
#define PHOTOPLANNERWINDOW_H

#include <QQuickWidget>
#include <QGeoCoordinate>
#include "photoplan.h"

class PhotoPlannerWindow : public QQuickWidget
{
    Q_OBJECT
    Q_PROPERTY(QVariantList aoi READ aoi WRITE setAoi NOTIFY aoiChanged)

public:
    PhotoPlannerWindow();
    QVariantList aoi() const;
    void setAoi(const QVariantList &aoi);

    Q_INVOKABLE void calculateAoI(QVariantList aoi);

signals:
    void aoiChanged();

public slots:
    bool aboutToQuit();

private:

    PhotoPlan photoPlan;
//    QVector<QGeoCoordinate> m_aoi;
};

#endif // PHOTOPLANNERWINDOW_H
