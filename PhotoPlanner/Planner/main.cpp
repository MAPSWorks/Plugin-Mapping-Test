#include "photoplannerdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PhotoPlannerDialog w;
    w.show();

    return a.exec();
}
