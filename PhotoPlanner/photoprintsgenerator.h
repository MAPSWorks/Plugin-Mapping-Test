#ifndef PHOTOPRINTSGENERATOR_H
#define PHOTOPRINTSGENERATOR_H

#include "photoplannercore.h"

namespace aero_photo {

class LinePhotoPrintsGenerator {
public:
    LinePhotoPrintsGenerator(const TargetArea &area) : targetArea_(area) {
    }

    PhotoPrints GeneratePhotoPrints(qreal Lx, qreal Ly) {
        PhotoPrints photoPrints;
        auto &&targetLine = targetArea_.GetBorder();
        for(int i = 1; i < targetLine.size(); i++) {
            auto linePhotoPrints = GeneratePhotoPrints(targetLine[i-1], targetLine[i], Lx, Ly);
            photoPrints.append(linePhotoPrints);
        }
        return photoPrints;
    }

private:
    PhotoPrints GeneratePhotoPrints(const GeoPoint &from, const GeoPoint &to, qreal Lx, qreal Ly) {
        PhotoPrints linePhotoPrints;
        auto azimuth = from.azimuthTo(to);
        const int totalPrints = ((from.distanceTo(to)+0.5) / Lx) + 1;
        auto fakeHalfDiag = sqrt(Lx*Lx+Ly*Ly)/2.0;
        for (int i = 0; i<totalPrints; ++i) {
            auto nextCenter = from.atDistanceAndAzimuth(Lx*i, azimuth);
            auto ppLT = nextCenter.atDistanceAndAzimuth(fakeHalfDiag, azimuth - 45);
            auto ppRT = nextCenter.atDistanceAndAzimuth(fakeHalfDiag, azimuth + 45);
            auto ppLB = nextCenter.atDistanceAndAzimuth(fakeHalfDiag, azimuth - 135);
            auto ppRB = nextCenter.atDistanceAndAzimuth(fakeHalfDiag, azimuth + 135);
            GeoPoints printBorder;
            printBorder << ppLT << ppRT << ppLB << ppRB;
            linePhotoPrints<<PhotoPrint(nextCenter, printBorder);
        }
        return linePhotoPrints;
    }

    TargetArea targetArea_;
};


}

#endif // PHOTOPRINTSGENERATOR_H
