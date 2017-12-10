#ifndef PHOTOPLANNERPLUGIN_H
#define PHOTOPLANNERPLUGIN_H

#include "src/shared/plugin_interface.h"
#include "photoplannerwindow.h"

class PhotoPlannerPlugin : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.uavos.gcu.PluginInterface/1.0")
    Q_INTERFACES(PluginInterface)
public:
    void init(void);
    bool aboutToQuit() {return static_cast<PhotoPlannerWindow*>(obj)->aboutToQuit();};
};

#endif // PHOTOPLANNERPLUGIN_H
