﻿/*
 * Copyright (C) 2011 Aliaksei Stratsilatau <sa@uavos.com>
 *
 * This file is part of the UAV Open System Project
 *  http://www.uavos.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "HttpService.h"
#include "QMandala.h"
#include "QMandala.h"
//=============================================================================
HttpService::HttpService(QObject* parent)
 : QObject(parent)
{
}
//=============================================================================
void HttpService::httpRequest(QTextStream &stream,QString req,bool *ok)
{
  if(req.startsWith("/kml")){
    stream << "HTTP/1.0 200 Ok\r\n";
    stream << (req.contains(".dae")?"application/xml dae":"application/vnd.google-earth.kml+xml");
    stream << "; charset=\"utf-8\"\r\n";
    stream << "\r\n";
    stream << reply_google(req.mid(4));
    *ok=true;
    return;
  }
  if(req.startsWith("/mandala")){
    stream << "HTTP/1.0 200 Ok\r\n";
    stream << "Content-Type: application/xml; charset=\"utf-8\"\r\n";
    stream << "\r\n";
    stream << reply_mandala(req.contains('?')?req.mid(req.indexOf("?")+1):"");
    *ok=true;
    return;
  }

}
//=============================================================================
//=============================================================================
/*void HttpService::readClient()
 {
  QTcpSocket* socket = (QTcpSocket*)sender();
  if (socket->canReadLine()) {
    QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
    if (tokens[0]=="GET") {
      QString req=QUrl::fromPercentEncoding(tokens[1].toUtf8());
      QTextStream os(socket);
      os.setAutoDetectUnicode(true);
      os << "HTTP/1.0 200 Ok\r\n";
      if(req.startsWith("/kml")){
        os << (req.contains(".dae")?"application/xml dae":"application/vnd.google-earth.kml+xml");
        os << "; charset=\"utf-8\"\r\n";
      }else os << "Content-Type: application/xml; charset=\"utf-8\"\r\n";
      os << "\r\n";
      os << reply(req) << "\n";
      socket->close();
      if (socket->state() == QTcpSocket::UnconnectedState)
        delete socket;
    }
  }
}
//=============================================================================
QString HttpService::reply(const QString &req)
{
  if(req.startsWith("/kml"))
    return reply_google(req.mid(4));
  //if(req.isEmpty()||req.startsWith("/mandala"))
    return reply_mandala(req.contains('?')?req.mid(req.indexOf("?")+1):"");
  return QString();
}*/
//=============================================================================
//=============================================================================
//=============================================================================
QString HttpService::reply_mandala(const QString &req)
{
  QString reply;
  QXmlStreamWriter xml(&reply);
  xml.setAutoFormatting(true);
  xml.writeStartDocument();
  xml.writeStartElement("mandala");
  //xml.writeTextElement("name","UAVOS GCU Mandala");
  //xml.writeTextElement("version",QMandala::version);
  //------------------------------
  bool doDescr=false;
  QStringList sv=req.split('&',QString::SkipEmptyParts);
  if(sv.isEmpty())sv=var->names;
  else{
    QStringList st;
    foreach(QString s,sv){
      if(s.startsWith("descr")){
        doDescr=true;
        s="";
      }else if(s.contains('=')){
        QString sname=s.left(s.indexOf('='));
        if(sname=="scr"){
          var->exec_script(s.mid(sname.size()+1));
          s="";
        }else{
          var->exec_script(QString("set('%1',%2)").arg(sname).arg(s.mid(sname.size()+1)));
          s=sname;
        }
      }
      if(s.size())st.append(s);
    }
    if(st.size())sv=st;
    else sv=var->names;
  }
  //vars
  foreach(QString vname,sv){
    QString s=QString("%1").arg(var->field(vname)->value(),0,'f',10);
    while(s.at(s.size()-1)=='0'){ //remove trailing zeros
      s.remove(s.size()-1,1);
      if(s.at(s.size()-1)!='.')continue;
      s.remove(s.size()-1,1);
      break;
    }
    if(doDescr){
      xml.writeStartElement(vname);
      xml.writeAttribute("descr",var->field(vname)->descr());
      xml.writeCharacters(s);
      xml.writeEndElement();
    }else{
      xml.writeTextElement(vname,s);
    }
  }
  //------------------------------
  xml.writeEndElement();//mandala
  xml.writeEndDocument();
  return reply;
}
//=============================================================================
//=============================================================================
QString HttpService::reply_google(const QString &req)
{
  if(req.isEmpty()||req=="/")return reply_kml();
  if(req=="/telemetry")return reply_telemetry();
  if(req=="/flightplan")return reply_flightplan();
  if(req=="/chase")return reply_chase();
  if(req=="/chase_upd")return reply_chase_upd();

  if(req.contains(".dae")){
    QFile f(QMandala::Global::res().filePath("bitmaps"+req));
    if(f.open(QIODevice::ReadOnly|QIODevice::Text))
      return f.readAll();
    qWarning("Model not found: %s",req.toUtf8().data());
  }
  return QString();
}
//=============================================================================
QString HttpService::reply_kml()
{
  QString reply;
  QXmlStreamWriter xml(&reply);
  xml.setAutoFormatting(true);
  xml.writeStartDocument();
  xml.writeStartElement("kml");
  xml.writeAttribute("xmlns","http://www.opengis.net/kml/2.2");
  xml.writeStartElement("Document");
  xml.writeTextElement("name","UAVOS GCU Data");
  //------------------------------
  //folder contents
  //------------------------------
  xml.writeStartElement("NetworkLink");
  xml.writeTextElement("name","Flight path from telemetry file");
  xml.writeStartElement("Link");
  xml.writeTextElement("href","/telemetry");
  xml.writeEndElement();//Link
  xml.writeEndElement();//NetworkLink
  //------------------------------
  xml.writeStartElement("NetworkLink");
  xml.writeTextElement("name","Chase cam view");
  xml.writeTextElement("visibility","0");
  xml.writeTextElement("flyToView","1");
  xml.writeStartElement("Link");
  xml.writeTextElement("href","/chase");
  xml.writeTextElement("refreshMode","onInterval");
  xml.writeTextElement("refreshInterval","1");
  xml.writeEndElement();//Link
  xml.writeEndElement();//NetworkLink

  //------------------------------
  xml.writeStartElement("NetworkLink");
  xml.writeTextElement("name","Flight Plan");
  xml.writeTextElement("visibility","0");
  xml.writeStartElement("Link");
  xml.writeTextElement("href","/flightplan");
  xml.writeTextElement("refreshMode","onInterval");
  xml.writeTextElement("refreshInterval","5");
  xml.writeEndElement();//Link
  xml.writeEndElement();//NetworkLink
  //------------------------------
  xml.writeEndElement();//Document
  xml.writeEndElement();//kml
  xml.writeEndDocument();
  return reply;
}
//=============================================================================
QString HttpService::reply_telemetry()
{
  QString reply;
  QXmlStreamWriter xml(&reply);
  xml.setAutoFormatting(true);
  xml.writeStartDocument();
  xml.writeStartElement("kml");
  xml.writeAttribute("xmlns","http://www.opengis.net/kml/2.2");
  xml.writeAttribute("xmlns:gx","http://www.google.com/kml/ext/2.2");
  xml.writeStartElement("Document");

  xml.writeStartElement("Style");
  xml.writeAttribute("id","traceStyle");
  xml.writeStartElement("LineStyle");
  xml.writeTextElement("color","7Fff1010");
  xml.writeTextElement("width","4");
  xml.writeEndElement();//LineStyle
  xml.writeStartElement("PolyStyle");
  xml.writeTextElement("color","7F801010");
  xml.writeEndElement();//PolyStyle
  xml.writeEndElement();//Style
  //------------------------------
  // FLIGHT TRACE
  xml.writeStartElement("Placemark");
  xml.writeTextElement("name",QFileInfo(var->rec->loadFile).baseName());
  xml.writeTextElement("styleUrl","#traceStyle");
  xml.writeTextElement("visibility","0");
  xml.writeTextElement("gx:balloonVisibility","0");
  xml.writeStartElement("LineString");
  xml.writeTextElement("tessellate","0");
  xml.writeTextElement("altitudeMode","absolute");

  //fill trace coordinates
  int igps_lat=var->names.indexOf("gps_lat");
  int igps_lon=var->names.indexOf("gps_lon");
  int igps_hmsl=var->names.indexOf("gps_hmsl");
  QStringList st;
  foreach(const FlightDataFile::ListDouble &vlist,var->rec->file.data){
    const double lat=vlist.at(igps_lat);
    const double lon=vlist.at(igps_lon);
    const double hmsl=vlist.at(igps_hmsl);
    st.append(QString().sprintf("%.6f,%.6f,%.1f",lon,lat,hmsl));
  }
  xml.writeTextElement("coordinates",st.join("\n"));
  xml.writeEndElement();//LineString
  xml.writeEndElement();//Placemark
  //------------------------------
  // GCU POS
  xml.writeStartElement("Placemark");
  xml.writeTextElement("name","GCU location");
  xml.writeTextElement("visibility","1");
  xml.writeTextElement("gx:balloonVisibility","0");
  xml.writeStartElement("Point");
  xml.writeTextElement("altitudeMode","relativeToGround");
  xml.writeTextElement("coordinates",QString().sprintf("%.6f,%.6f,%.1f",var->rec->value("gps_home_lon"),var->rec->value("gps_home_lat"),0.0));
  xml.writeEndElement();//Point
  xml.writeEndElement();//Placemark
  //------------------------------
  // UAV MODEL
  xml.writeStartElement("Placemark");
  xml.writeAttribute("id","modelUavPM");
  xml.writeTextElement("name","UAV model");
  xml.writeTextElement("visibility","1");
  xml.writeTextElement("gx:balloonVisibility","0");
  xml.writeStartElement("Model");
  xml.writeTextElement("altitudeMode","absolute");
  xml.writeStartElement("Location");
  xml.writeAttribute("id","uavLoc");
  xml.writeTextElement("longitude",QString().sprintf("%.6f",var->rec->value("gps_lon")));
  xml.writeTextElement("latitude",QString().sprintf("%.6f",var->rec->value("gps_lat")));
  xml.writeTextElement("altitude",QString().sprintf("%.1f",var->rec->value("gps_hmsl")));
  xml.writeEndElement();//Location
  xml.writeStartElement("Orientation");
  xml.writeAttribute("id","uavOri");
  xml.writeTextElement("heading",0);
  xml.writeTextElement("tilt",0);
  xml.writeTextElement("roll",0);
  xml.writeEndElement();//Orientation
  xml.writeStartElement("Link");
  xml.writeTextElement("href","/models/uav.dae");
  xml.writeEndElement();//Link
  xml.writeEndElement();//Model
  xml.writeEndElement();//Placemark

  //------------------------------
  // TOUR FLYTO
  xml.writeStartElement("gx:Tour");
  xml.writeTextElement("name","Flight tour");
  xml.writeStartElement("gx:Playlist");
  int iroll=var->names.indexOf("roll");
  int ipitch=var->names.indexOf("pitch");
  int iyaw=var->names.indexOf("yaw");
  uint time_s=0;
  int i=-1,cam_i=-1;
  foreach(uint time,var->rec->file.time){
    i++;
    if(cam_i>=0)cam_i++;
    //every second
    if((time-time_s)<1000)continue;
    time_s=time;
    const FlightDataFile::ListDouble &vlist=var->rec->file.data.at(i);
    //find camera
    if(cam_i>=0){  //delay camera 2sec
      FlightDataFile::ListDouble vlist_cam=var->rec->file.data.at(cam_i);
      xml.writeStartElement("gx:FlyTo");
      xml.writeTextElement("gx:flyToMode","smooth");
      xml.writeTextElement("gx:duration","1");  //1sec
      xml.writeStartElement("Camera");
      xml.writeTextElement("longitude",QString().sprintf("%.6f",vlist_cam.at(igps_lon)));
      xml.writeTextElement("latitude",QString().sprintf("%.6f",vlist_cam.at(igps_lat)));
      xml.writeTextElement("altitude",QString().sprintf("%.1f",vlist_cam.at(igps_hmsl)));
      double crs=vlist_cam.at(iyaw);
      xml.writeTextElement("heading",QString().sprintf("%.0f",crs<0?crs+360.0:crs));
      xml.writeTextElement("tilt",QString().sprintf("%.0f",vlist_cam.at(ipitch)+90.0));
      xml.writeTextElement("roll",QString().sprintf("%.0f",-vlist_cam.at(iroll)));
      xml.writeTextElement("altitudeMode","absolute");
      xml.writeEndElement();//Camera
      xml.writeEndElement();//FlyTo
    }else if(time>=3000) cam_i=0;

    xml.writeStartElement("gx:AnimatedUpdate");
    xml.writeTextElement("gx:duration","1");    //1sec
    xml.writeStartElement("Update");
    xml.writeTextElement("targetHref","");
    xml.writeStartElement("Change");
    xml.writeStartElement("Location");
    xml.writeAttribute("targetId","uavLoc");
    xml.writeTextElement("longitude",QString().sprintf("%.6f",vlist.at(igps_lon)));
    xml.writeTextElement("latitude",QString().sprintf("%.6f",vlist.at(igps_lat)));
    xml.writeTextElement("altitude",QString().sprintf("%.1f",vlist.at(igps_hmsl)));
    xml.writeEndElement();//Location
    xml.writeStartElement("Orientation");
    xml.writeAttribute("targetId","uavOri");
    xml.writeTextElement("heading",QString().sprintf("%.0f",var->boundAngle360(vlist.at(iyaw)+180.0)));
    xml.writeTextElement("tilt",QString().sprintf("%.0f",vlist.at(ipitch)));
    xml.writeTextElement("roll",QString().sprintf("%.0f",vlist.at(iroll)));
    xml.writeEndElement();//Orientation
    xml.writeEndElement();//Change
    xml.writeEndElement();//Update
    xml.writeEndElement();//AnimatedUpdate

    //xml.writeStartElement("gx:Wait");
    //xml.writeTextElement("gx:duration","1");
    //xml.writeEndElement();//Wait
  }
  xml.writeEndElement();//Playlist
  xml.writeEndElement();//Tour
  //------------------------------
  // TRACK TIMELINE
  xml.writeStartElement("Placemark");
  xml.writeTextElement("name","Flight track timeline");
  xml.writeTextElement("visibility","1");
  xml.writeTextElement("gx:balloonVisibility","0");
  xml.writeStartElement("Style");
  xml.writeStartElement("IconStyle");
  xml.writeStartElement("Icon");
  xml.writeEndElement();
  xml.writeEndElement();
  xml.writeStartElement("LabelStyle");
  xml.writeTextElement("scale","0");
  xml.writeEndElement();
  xml.writeEndElement();
  xml.writeStartElement("gx:Track");
  xml.writeTextElement("altitudeMode","absolute");
  xml.writeTextElement("gx:horizFov","10");
  QDateTime t=var->rec->file.timestamp;
  time_s=0;
  i=-1;
  foreach(uint time,var->rec->file.time){
    i++;
    //every second
    if((time-time_s)<1000)continue;
    time_s=time;
    const FlightDataFile::ListDouble &vlist=var->rec->file.data.at(i);
    xml.writeTextElement("when",t.toString("yyyy-MM-ddTHH:mm:ssZ"));
    xml.writeTextElement("gx:coord",QString().sprintf("%.6f,%.6f,%.1f",vlist.at(igps_lon),vlist.at(igps_lat),vlist.at(igps_hmsl)));
    double crs=var->boundAngle(vlist.at(iyaw)+180.0);
    xml.writeTextElement("gx:angles",QString().sprintf("%.1f,%.1f,%.1f",crs,vlist.at(ipitch),vlist.at(iroll)));
    t=t.addSecs(1);
  }
  xml.writeStartElement("Model");
  xml.writeStartElement("Link");
  xml.writeTextElement("href","/models/uav.dae");
  xml.writeEndElement();//Link
  xml.writeEndElement();//Model

  xml.writeEndElement();//Track
  xml.writeEndElement();//Placemark
  //------------------------------



  xml.writeEndElement();//Document
  xml.writeEndElement();//kml
  xml.writeEndDocument();
  return reply;
}
//=============================================================================
QString HttpService::reply_flightplan()
{
  QString reply;
  QXmlStreamWriter xml(&reply);
  xml.setAutoFormatting(false);
  xml.writeStartDocument();
  xml.writeStartElement("kml");
  xml.writeAttribute("xmlns","http://www.opengis.net/kml/2.2");
  xml.writeStartElement("Document");

  xml.writeStartElement("Style");
  xml.writeAttribute("id","traceStyle");
  xml.writeStartElement("LineStyle");
  xml.writeTextElement("color","7F303010");
  xml.writeTextElement("width","4");
  xml.writeEndElement();//LineStyle
  xml.writeEndElement();//Style
  //------------------------------
  // waypoints
  xml.writeStartElement("Folder");
  xml.writeTextElement("name","Waypoints");
  /*for(uint i=0;i<var->wpcnt;i++){
    xml.writeStartElement("Placemark");
    xml.writeTextElement("name","W"+QString::number(i+1));
    xml.writeTextElement("visibility","1");
    xml.writeStartElement("Point");
    xml.writeTextElement("altitudeMode","absolute");
    xml.writeTextElement("coordinates",QString().sprintf("%.6f,%.6f,%.0f",var->fp.waypoints[i].LLA[1],var->fp.waypoints[i].LLA[0],var->home_pos[2]+var->fp.waypoints[i].LLA[2]));
    xml.writeEndElement();//Point
    xml.writeEndElement();//Placemark
    }*/
  xml.writeEndElement();//Folder

  //------------------------------
  //path
  xml.writeStartElement("Placemark");
  xml.writeTextElement("name","Flight Plan path");
  xml.writeTextElement("styleUrl","#traceStyle");
  xml.writeTextElement("gx:balloonVisibility","0");
  xml.writeStartElement("LineString");
  xml.writeTextElement("tessellate","0");
  xml.writeTextElement("altitudeMode","absolute");
  QStringList st;
  /*for(uint i=0;i<var->wpcnt;i++)
    st.append(QString().sprintf("%.6f,%.6f,%.1f",var->fp.waypoints[i].LLA[1],var->fp.waypoints[i].LLA[0],var->home_pos[2]+var->fp.waypoints[i].LLA[2]));
  xml.writeTextElement("coordinates",st.join("\n"));*/
  xml.writeEndElement();//LineString
  xml.writeEndElement();//Placemark
  //------------------------------
  xml.writeEndElement();//Document
  xml.writeEndElement();//kml
  xml.writeEndDocument();
  return reply;
}
//=============================================================================
QString HttpService::reply_chase()
{
  QString reply;
  QXmlStreamWriter xml(&reply);
  xml.setAutoFormatting(false);
  xml.writeStartDocument();
  xml.writeStartElement("kml");
  xml.writeAttribute("xmlns","http://www.opengis.net/kml/2.2");
  xml.writeStartElement("Document");
  //------------------------------
  //------------------------------
  //xml.writeStartElement("gx:Tour");
  //xml.writeTextElement("name","cam tour");
  //xml.writeStartElement("gx:Playlist");
  //xml.writeAttribute("id","camPlayList");

  //xml.writeStartElement("gx:FlyTo");
  //xml.writeTextElement("gx:flyToMode","smooth");
  //xml.writeTextElement("gx:duration","5.0");
  xml.writeStartElement("Camera");
  xml.writeAttribute("id","camChase");
  xml.writeTextElement("longitude",QString().sprintf("%.6f",var->gps_pos[1]));
  xml.writeTextElement("latitude",QString().sprintf("%.6f",var->gps_pos[0]));
  xml.writeTextElement("altitude",QString().sprintf("%.1f",var->gps_pos[2]));
  xml.writeTextElement("heading",QString().sprintf("%.0f",var->course<0?var->course+360.0:var->course));
  xml.writeTextElement("tilt",QString().sprintf("%.0f",var->theta[1]+90.0));
  xml.writeTextElement("roll",QString().sprintf("%.0f",-var->theta[0]));
  xml.writeTextElement("altitudeMode","absolute");
  xml.writeEndElement();//Camera
  //xml.writeEndElement();//FlyTo

  //xml.writeEndElement();//Playlist
  //xml.writeEndElement();//Tour
  //------------------------------
  //------------------------------
  xml.writeEndElement();//Document
  xml.writeEndElement();//kml
  xml.writeEndDocument();
  return reply;
}
QString HttpService::reply_chase_upd()
{
  QString reply;
  QXmlStreamWriter xml(&reply);
  xml.setAutoFormatting(false);
  xml.writeStartDocument();
  xml.writeStartElement("kml");
  xml.writeAttribute("xmlns","http://www.opengis.net/kml/2.2");
  //------------------------------
  xml.writeStartElement("NetworkLinkControl");
  //xml.writeStartElement("gx:AnimatedUpdate");
  //xml.writeTextElement("gx:duration","2.0");
  xml.writeStartElement("Update");
  xml.writeTextElement("targetHref","/chase");
  xml.writeStartElement("Create");

  xml.writeStartElement("gx:Playlist");
  xml.writeAttribute("id","camPlayList");
  xml.writeStartElement("gx:FlyTo");
  xml.writeTextElement("gx:flyToMode","smooth");
  xml.writeTextElement("gx:duration","2.0");
  xml.writeStartElement("Camera");
  xml.writeAttribute("id","camChase");
  xml.writeTextElement("longitude",QString().sprintf("%.6f",var->gps_pos[1]));
  xml.writeTextElement("latitude",QString().sprintf("%.6f",var->gps_pos[0]));
  xml.writeTextElement("altitude",QString().sprintf("%.1f",var->gps_pos[2]));
  xml.writeTextElement("heading",QString().sprintf("%.0f",var->course<0?var->course+360.0:var->course));
  xml.writeTextElement("tilt",QString().sprintf("%.0f",var->theta[1]+90.0));
  xml.writeTextElement("roll",QString().sprintf("%.0f",-var->theta[0]));
  xml.writeTextElement("altitudeMode","absolute");
  xml.writeEndElement();//Camera
  xml.writeEndElement();//FlyTo
  xml.writeEndElement();//Playlist

  xml.writeEndElement();//Create
  xml.writeEndElement();//Update
  //xml.writeEndElement();//AnimatedUpdate
  xml.writeEndElement();//NetworkLinkControl
  //------------------------------
  xml.writeEndElement();//kml
  xml.writeEndDocument();
  return reply;
}
//=============================================================================

