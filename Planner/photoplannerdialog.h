#ifndef PHOTOPLANNERDIALOG_H
#define PHOTOPLANNERDIALOG_H

#include <QDialog>

namespace Ui {
class PhotoPlannerDialog;
}

class PhotoPlannerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PhotoPlannerDialog(QWidget *parent = 0);
    ~PhotoPlannerDialog();

private:
    Ui::PhotoPlannerDialog *ui;
};

#endif // PHOTOPLANNERDIALOG_H
