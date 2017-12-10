#include "photoplannerdialog.h"
#include "ui_photoplannerdialog.h"

#include <QDir>
#include <QString>
#include <QPluginLoader>
#include <QQuickWidget>
#include "src/shared/plugin_interface.h"

PhotoPlannerDialog::PhotoPlannerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PhotoPlannerDialog)
{
    ui->setupUi(this);

    QDir pluginsDir(QApplication::applicationDirPath()+"/plugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
                PluginInterface* pi = qobject_cast<PluginInterface*>(plugin);
                pi->init();
                QObject* obj = pi->obj;
                ui->verticalLayout->addWidget((QQuickWidget*)obj);
        }
    }
}

PhotoPlannerDialog::~PhotoPlannerDialog()
{
    delete ui;
}
