#ifndef PHOTOPLAN_PLUGIN_H
#define PHOTOPLAN_PLUGIN_H

#include <QQmlExtensionPlugin>

class PhotoPlanPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    void registerTypes(const char *uri);
};

#endif // PHOTOPLAN_PLUGIN_H
