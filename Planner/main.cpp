#include "photoplannerdialog.h"
#include <QApplication>

#include "/home/user/Projects/mapping/src/shared/QMandala.h"

//QMandala *mandala;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


   // QMandala *mandala = new QMandala();

    PhotoPlannerDialog w;
    w.show();

    return a.exec();
}
