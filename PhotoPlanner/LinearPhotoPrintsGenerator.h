#ifndef LINEARPHOTOPRINTSGENERATOR_H
#define LINEARPHOTOPRINTSGENERATOR_H

#include "PhotoPlannerCore.h"
#include "LinedGeoPoints.h"
#include "LinePhotoPrintsGenerator.h"

#include <stdexcept>

namespace aero_photo {

class LinearPhotoPrintsGenerator {
public:
  explicit LinearPhotoPrintsGenerator(const LinearPhotoRegion &linearRegion) {
    if (linearRegion.GetTrack().size() < 2)
      throw std::logic_error("Invalid LinearPhotoRegion size()");
    trackHead_ = linearRegion.GetTrack().front();
    trackTail_ = linearRegion.GetTrack();
    trackTail_.pop_front();
  }

  LinedGeoPoints GeneratePhotoPrintsCenters(qreal h, qreal Lxp, qreal Lyp, size_t totalRuns) {
    trackHead_.setAltitude(h);
    for (auto &tailPnt : trackTail_)
      tailPnt.setAltitude(h);

    LinedGeoPoints linedGeoPoints(totalRuns * (trackTail_.size()));
    // !!! Both pntA & pntB must be copy of members
    auto pntA = trackHead_;
    for (int trackIndex = 0; trackIndex < trackTail_.size(); trackIndex++) {
      auto pntB = trackTail_[trackIndex];
      GeoCalc geoCalc;
      const auto distance = geoCalc.RoundUpPoints(pntA, pntB, Lxp);
      RunStartPointsCalc pointsA(pntA, pntB, Lyp, totalRuns);
      RunStartPointsCalc pointsB(pntB, geoCalc.Azimuth(pntA, pntB), Lyp, totalRuns);
      for (size_t runIndex = 0; runIndex < totalRuns; runIndex++) {
        auto startPointAB = pointsA.Calculate(runIndex);
        auto finishPointAB = pointsB.Calculate(runIndex);
        auto runAzimuth = geoCalc.Azimuth(startPointAB, finishPointAB);
        LinePhotoPrintsGenerator thisRunLineGenerator(startPointAB, runAzimuth, distance);
        auto pointRunPrintsCenters = thisRunLineGenerator.GeneratePhotoPrintsCenters(Lxp);
        if (runIndex % 2 != 0) {
          runAzimuth += 180;
          if (!pointRunPrintsCenters.empty())
            for (int i = 0, j = pointRunPrintsCenters.size() - 1; i < j; ++i, --j)
              std::swap(pointRunPrintsCenters[i], pointRunPrintsCenters[j]);
        }
        // We must calculate index
        const auto linedIndex = CalculateRunSequnce(trackIndex, runIndex);
        linedGeoPoints[linedIndex] = pointRunPrintsCenters;
        linedGeoPoints.SetAzimuth(linedIndex, runAzimuth);
      }
      pntA = trackTail_[trackIndex];
    }
    return linedGeoPoints;
  }

  PhotoPrints GeneratePhotoPrints(const LinedGeoPoints &linedGeoPoints, qreal Lx, qreal Ly) {
    PhotoPrints photoPrints;
    //photoPrints.reserve(linedGeoPoints.size());
    for (auto &&linePoints : linedGeoPoints) {
      LinePhotoPrintsGenerator thisRunLineGenerator(linePoints.front(), linePoints.back());
      auto thisRunLinePhotoPrints = thisRunLineGenerator.GeneratePhotoPrints(linePoints, Lx, Ly);
      photoPrints.append(thisRunLinePhotoPrints);
    }
    photoPrints.squeeze();
    return photoPrints;
  }

  int CalculateRunSequnce(int trackIndex, int runIndex) {
    const auto totalTracks = trackTail_.size();
    auto sequence =
      ((runIndex + 1) % 2) * trackIndex +
      (runIndex % 2) * (totalTracks - 1 - trackIndex) +
      runIndex * totalTracks;
    return sequence;
  }

private:
  GeoPoint trackHead_;
  GeoPoints trackTail_;
};

} // aero_photo

#endif // LINEARPHOTOPRINTSGENERATOR_H


