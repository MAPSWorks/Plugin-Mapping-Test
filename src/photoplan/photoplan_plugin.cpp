#include "photoplan_plugin.h"
#include "photoplanmodel.h"

#include <qqml.h>

void PhotoPlanPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<PhotoPlanModel>("PhotoPlan", 1, 0, "PhotoPlanModel");
}
