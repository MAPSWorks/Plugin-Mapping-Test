#ifndef PHOTOPLANNER_H
#define PHOTOPLANNER_H

#include "PhotoPlannerCore.h"
#include "PhotoCameraModel.h"
#include "PhotoUavModel.h"
#include "LinedGeoPoints.h"
#include "ManeuverTrackAlignment.h"

#include <QDateTime>
#include <QXmlStreamWriter>
#include <QFile>

namespace aero_photo {

//class Requirements {
//public:
//    double lx_ = 0;
//    double ly_ = 0;
//    double altitude_ = 0;
//};

//class Conditions {
//public:
//    double windSpeed = 0;
//    double windBearing = 0;
//};


class PhotoPlanner {
protected:
    PhotoPlanner(const PhotoUavModel &photoUav, const PhotoCameraModel &photoCamera)
        : photoUav_(photoUav)
        , photoCamera_(photoCamera) {
    }

public:
    virtual ~PhotoPlanner() {}

    bool IsCalculated() const { return isCalculated; }
    const PhotoPrints &GetPhotoPrints() const { return photoPrints_; }
    const GeoPoints &GetTrackPoints() const { return trackPoints_; }
    const FlightPoints &GetFlightPoints() const  { return flightPoints_; }

    qreal velocity() const { return photoUav_.velocity(); }

    void SaveToXml(QString fileurlcvt, const int totalPointsInPacket) {
        fileurlcvt = fileurlcvt.remove(".xml");
        for(int i = 0; totalPointsInPacket * i < GetFlightPoints().size(); i++) {
            auto packname = QString("%1-%2.xml").arg(fileurlcvt).arg(i);
            if (i == 0) {
                packname = QString("%1.xml").arg(fileurlcvt);
            }
            QFile xmlFile(packname);
            if (xmlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QXmlStreamWriter stream(&xmlFile);
                stream.setAutoFormatting(true);
                stream.writeStartDocument();
                SaveToXmlPacket(stream, i, totalPointsInPacket);
                stream.writeEndDocument();
            }
        }
    }

protected:
    void SaveToXmlPacket(QXmlStreamWriter &stream, int packetNumber, int totalPointsInPacket) {
        stream.writeStartElement("mission");
        stream.writeAttribute("version", "9.10.56");
        stream.writeAttribute("href", "http://www.uavos.com/");
        stream.writeAttribute("title", "mission");
        stream.writeTextElement("hash", "f7c8603a8b1af004375ebc326a0ab6e");
        stream.writeTextElement("timestamp", QDateTime::currentDateTime().toString());
        if (packetNumber==0 && !GetFlightPoints().empty())
            SaveToXmlHome(stream, GetFlightPoints().front());
        SaveToXmlPacketWayPoints(stream, packetNumber, totalPointsInPacket);
        stream.writeEndElement();
    }
    void SaveToXmlHome(QXmlStreamWriter &stream, const GeoPoint &homePoint) {
        stream.writeStartElement("home");
        stream.writeTextElement("lat", QString::number(homePoint.latitude(), 'f', 15));
        stream.writeTextElement("lon", QString::number(homePoint.longitude(), 'f', 15));
        stream.writeTextElement("hmsl", "0");
        stream.writeEndElement();
    }
    void SaveToXmlPacketWayPoints(QXmlStreamWriter &stream, int packetNumber, int totalPointsInPacket) {
        stream.writeStartElement("waypoints");
        const int packetOffset = packetNumber * totalPointsInPacket;
        totalPointsInPacket = std::min(totalPointsInPacket, GetFlightPoints().size() - packetOffset);
        stream.writeAttribute("cnt", QString::number(totalPointsInPacket));
        const auto &points = GetFlightPoints();
        for (int index = 0; index < totalPointsInPacket ; index ++) {
            SaveToXmlOnePoint(stream, index, points[packetOffset + index]);
        }
        stream.writeEndElement();
    }
    void SaveToXmlOnePoint(QXmlStreamWriter &stream, int index, const FlightPoint &flightPoint) {
        // Conversion???
        stream.writeStartElement("waypoint");
        stream.writeAttribute("idx", QString::number(index));
        stream.writeTextElement("altitude", QString::number(flightPoint.altitude()));
        stream.writeTextElement("type", QString::number(flightPoint.type()));
        stream.writeTextElement("latitude", QString::number(flightPoint.latitude(), 'f', 15));
        stream.writeTextElement("longitude", QString::number(flightPoint.longitude(), 'f', 15));
        SaveToXmlOnePointActions(stream, flightPoint);
        stream.writeEndElement();
    }

    void SaveToXmlOnePointActions(QXmlStreamWriter &stream, const FlightPoint &flightPoint) {
        stream.writeStartElement("actions");
        stream.writeTextElement("speed", QString::number(velocity()));
        stream.writeTextElement("shot", (flightPoint.shotDistance()>0) ? "2" : "0" );
        stream.writeTextElement("dshot", QString::number(flightPoint.shotDistance()));
        stream.writeTextElement("POI", "0");
        stream.writeTextElement("loiter", "0");
        stream.writeTextElement("turnR", "0");
        stream.writeTextElement("loops", "0");
        stream.writeTextElement("time", "0");
        stream.writeEndElement();
    }

    void CalculateTrack(double Bx) {
        trackPoints_.clear();
        flightPoints_.clear();

        const auto R = photoUav_.GetManeuverR();

        int prevLine = -1;
        for(int i = 0; i<linedGeoPoints_.size(); ++i) {
            auto &line = linedGeoPoints_[i];
            if(line.empty())
                continue;

            const bool enlargeEntryRequired = (R!=0) &&
                    CalculationParams::Instance().enlargeEntryRequired &&
                    (linedGeoPoints_[i].GetAdjustments() & PlannedTrackLine::AdjustmentEnlargeEntry);

            auto endAligmentPoint = linedGeoPoints_[i].front();
            auto additionalEntryStart = endAligmentPoint.atDistanceAndAzimuth(4*R, linedGeoPoints_[i].GetAzimuth() + 180);
            auto additionalEntryEnd = endAligmentPoint.atDistanceAndAzimuth(2*R, linedGeoPoints_[i].GetAzimuth() + 180);
            if (enlargeEntryRequired) {
                endAligmentPoint = additionalEntryStart;
            }

            if(prevLine>=0) {
                auto aligmentMode = linedGeoPoints_[i].GetAdjustments() & PlannedTrackLine::AlignmentsMask;
                switch(aligmentMode) {
                case PlannedTrackLine::AdjustmentByUav: {
                    if (!trackPoints_.empty() && !linedGeoPoints_[i].empty() &&
                            linedGeoPoints_[i].front().distanceTo(trackPoints_.back()) < 10) {
                        trackPoints_.pop_back();
                        flightPoints_.pop_back();
                    }
                    break;
                }
                case PlannedTrackLine::AdjustmentAlignByManeuver: {
                    ManeuverTrackAlignment aligment(linedGeoPoints_[prevLine].back(), linedGeoPoints_[prevLine].GetAzimuth(), endAligmentPoint, linedGeoPoints_[i].GetAzimuth());
                    auto aligmentPoints = aligment.Calculate(photoUav_);
                    for(auto point : aligmentPoints)
                        trackPoints_.push_back(point);
                    for(auto point : aligment.GetFlightPoints())
                        flightPoints_.push_back(FlightPoint(point, 0));
                    break;
                }
                }
            }

            if (enlargeEntryRequired) {
                trackPoints_.push_back(additionalEntryStart);
                trackPoints_.push_back(additionalEntryEnd);
                flightPoints_.push_back(FlightPoint(additionalEntryStart, 0));
                flightPoints_.push_back(FlightPoint(additionalEntryEnd, 1));
            }

            trackPoints_.push_back(line.front());
            flightPoints_.push_back(FlightPoint(line.front(), 0, Bx));
            if(line.size()>1) {
                trackPoints_.push_back(line.back());
                flightPoints_.push_back(FlightPoint(line.back(), 1));
            }
            prevLine = i;
        }

        isCalculated = true;
    }

    bool isCalculated = false;
    PhotoUavModel photoUav_;
    PhotoCameraModel photoCamera_;
    LinedGeoPoints linedGeoPoints_;
    GeoPoints trackPoints_;
    FlightPoints flightPoints_;
    PhotoPrints photoPrints_;
};



}

#endif // PHOTOPLANNER_H
