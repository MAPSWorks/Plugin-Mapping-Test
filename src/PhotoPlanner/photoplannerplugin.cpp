#include "photoplannerplugin.h"

#include "../shared/QMandala.h"

QMandala *mandala;

void PhotoPlannerPlugin::init(void)
{
    QMandala *mandala = new QMandala();
    Q_UNUSED(mandala);
    obj = new PhotoPlannerWindow();
}
