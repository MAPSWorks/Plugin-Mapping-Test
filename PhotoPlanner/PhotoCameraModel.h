#ifndef PHOTOCAMERAMODEL_H
#define PHOTOCAMERAMODEL_H

namespace aero_photo {

class PhotoCameraModel {
public:
    PhotoCameraModel(double f, double lx, double ly) : f_(f), lx_(lx), ly_(ly) {
    }

    void CalcLxLy(double h, double &Lx, double &Ly) {
        const double m = CalcM(h);
        Lx = lx_ * m;
        Ly = ly_ * m;
    }

    void CalcBxBy(double h, double Px, double Py, double &Bx, double &By) {
        CalcLxLy(h, Bx, By);
        Bx *= (100.0 - Px) / 100.0;
        By *= (100.0 - Py) / 100.0;
    }

private:
    inline double CalcM(double h) {
        return h/f_;
    }

    const double f_;
    const double lx_, ly_;
};

} // aero_photo

#endif // PHOTOCAMERAMODEL_H
