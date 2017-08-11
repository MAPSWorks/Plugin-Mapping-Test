#include "photoplan_plugin.h"
#include "geocoordinatesmodel.h"
#include "photoplan.h"
#include <qqml.h>

void PhotoPlanPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<GeoCoordinatesModel>("PhotoPlan", 1, 0, "PhotoPlanModel");
    qmlRegisterType<PhotoPlan>("PhotoPlan", 1, 0, "QMLPhotoPlan");
}
