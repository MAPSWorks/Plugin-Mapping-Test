import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.2
import QtLocation 5.9
import QtPositioning 5.8

Map {
    id: map
    property variant markers
    property variant mapItems

    property int markerCounter:     0 // counter for total amount of markers. Resets to 0 when number of markers = 0
    property int currentMarker
    property int lastX :            -1
    property int lastY :            -1
    property int currX :            -1
    property int currY :            -1
    property int pressX :           -1
    property int pressY :           -1
    property int jitterThreshold :  30
    property bool followme:         false
    property variant scaleLengths:  [5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000, 2000000]
    property string missionType

    property MapPolyline            linearPoI
    property MapPolygon             areaPoI
    property MapPolyline            track
    property var trackMarkers: []

    property var photoPrints: []
    property bool photoPrintsVisible: false

    signal showMainMenu(variant coordinate)
    signal showMarkerMenu(variant coordinate)



    function calculateScale()
    {
      var coord1, coord2, dist, text, f
      f = 0
      coord1 = map.toCoordinate(Qt.point(0,scale.y))
      coord2 = map.toCoordinate(Qt.point(0+scaleImage.sourceSize.width,scale.y))
      dist = Math.round(coord1.distanceTo(coord2))
      if (dist === 0) {
          // not visible
      } else {
          for (var i = 0; i < scaleLengths.length-1; i++) {
              if (dist < (scaleLengths[i] + scaleLengths[i+1]) / 2 ) {
                  f = scaleLengths[i] / dist
                  dist = scaleLengths[i]
                  break;
              }
          }
          if (f === 0) {
              f = dist / scaleLengths[i]
              dist = scaleLengths[i]
          }
      }
      text = dist
      scaleImage.width = (scaleImage.sourceSize.width * f) - 2 * scaleImageLeft.sourceSize.width
      scaleText.text = text
    }

    function addMarker()
    {
         var count = map.markers.length
         markerCounter++
         var marker = Qt.createQmlObject ('Marker {}', map)
         map.addMapItem(marker)
         marker.z = map.z+1
         marker.coordinate = mouseArea.lastCoordinate
         //update list of markers
         var myArray = new Array()
         for (var i = 0; i<count; i++){
             myArray.push(markers[i])
         }
         myArray.push(marker)
         markers = myArray
         if(missionType === "Area")
         {
   //          photoPlanner.calcAreaPhotoPrints(myArray);
             map.addGeoItem("PolygonItem")
         }
         else //pathLength
         {
     //        photoPlanner.calcLinearPhotoPrints(myArray);
             map.addGeoItem("PolylineItem")

         }

//         var photoPrint = Qt.createQmlObject('PhotoPrint {}', map)
//         photoPrint.setBorder(
//                     mouseArea.lastCoordinate,
//                     mouseArea.lastCoordinate.atDistanceAndAzimuth(1000, 90),
//                     mouseArea.lastCoordinate.atDistanceAndAzimuth(2000, 180),
//                     mouseArea.lastCoordinate.atDistanceAndAzimuth(3000, 235));
//         map.addMapItem(photoPrint)
    }

    function addCoordinateMarker(coordinate)
    {
         var count = map.markers.length
         markerCounter++
         var marker = Qt.createQmlObject ('Marker {}', map)
         map.addMapItem(marker)
         marker.z = map.z+1
         marker.coordinate = coordinate
         //update list of markers
         var myArray = new Array()
         for (var i = 0; i<count; i++){
             myArray.push(markers[i])
         }
         myArray.push(marker)
         markers = myArray
         if(missionType === "Area")
         {
   //          photoPlanner.calcAreaPhotoPrints(myArray);
             map.addGeoItem("PolygonItem")
         }
         else //pathLength
         {
     //        photoPlanner.calcLinearPhotoPrints(myArray);
             map.addGeoItem("PolylineItem")

         }
    }

    function deleteMarker(index)
    {
         //update list of markers
        clearTrack();
         var myArray = new Array()
         var count = map.markers.length
         for (var i = 0; i<count; i++){
             if (index != i) myArray.push(map.markers[i])
         }
         map.removeMapItem(map.markers[index])
         map.markers[index].destroy()
         map.markers = myArray
        if (markers.length == 0) markerCounter = 0

         if(missionType === "Area")
         {
             map.addGeoItem("PolygonItem")
         }
         else
         {
             map.addGeoItem("PolylineItem")
         }
    }

    function deleteMarkers()
    {
        clearTrack();

        var count = map.markers.length
        for (var i = 0; i<count; i++){
             map.removeMapItem(map.markers[i])
             map.markers[i].destroy()
        }
        map.markers = []
        markerCounter = 0
    }

    function addGeoItem(item)
    {
        //mapItems = new Array();
        var count = map.mapItems.length
        var co = Qt.createComponent(item+'.qml')
        if (co.status == Component.Ready) {
            var o = co.createObject(map)
            o.setGeometry(map.markers, currentMarker)
            map.addMapItem(o)
            //update list of items
            var myArray = new Array()
            for (var i = 0; i<count; i++){
                map.removeMapItem(mapItems[i]);
                //myArray.push(mapItems[i])
            }

            myArray.push(o)
            mapItems = myArray

        } else {
            console.log(item + " is not supported right now, please call us later.")
        }
    }

    function calculatePhotoPlan () {

        var myArray = new Array()
        for (var i = 0; i<markers.length; i++){
            myArray.push(markers[i].coordinate)
        }

        if(missionType === "Area")
        {
            photoPlanner.calcAreaPhotoPrints(myArray);
            map.addGeoItem("PolygonItem")
    /*        for(var i=0; i < photoPlanner.getFlightPointCount(); i++)
            {
                map.addCoordinateMarker(photoPlanner.getFlightPointCoord(i));
            }
      */
        }
        else //pathLength
        {
            photoPlanner.calcLinearPhotoPrints(myArray);
            map.addGeoItem("PolylineItem")

        }

        clearPhotoPrints();
        clearTrack();
        clearTrackMarkers();

        map.track = Qt.createQmlObject('import QtLocation 5.6; MapPolyline {}', item);
        map.track.line.width = 2;
        map.track.line.color = 'blue';
        map.addMapItem(map.track);
        map.track.path = photoPlanner.track; //map.areaPoI.path //photoPlanner.track;
        addTrackMarkers();

        var pathLength = 0;
        for(var i =0; i < map.track.pathLength() -1; i++)
        {
            pathLength+=map.track.path[i].distanceTo(map.track.path[i+1]);
        }

        if (photoPrintsVisible)
            addPhotoPrints();

        resultsDlg.flightPathLength.text = (pathLength/1000+0.5).toFixed(1);
        resultsDlg.flightSpeed.text = photoPlanner.speed;
        resultsDlg.flightTime.text = (pathLength*60/(1000*photoPlanner.speed)+0.5).toFixed(0);
        resultsDlg.picturesCount.text = photoPlanner.photoCenters.length;
        resultsDlg.open();
    }


    onPhotoPrintsVisibleChanged: {
        if (photoPrintsVisible)
            addPhotoPrints()
        else
            clearPhotoPrints()
    }

    function addPhotoPrints()
    {
        var startTime = new Date()
        var newPhotoPrints = [];
        var photoprintspoints = photoPlanner.photoPrints;
        var photoPrintsCount = photoprintspoints.length/4;
        var getPhotoPrintsTime = new Date()
        console.log("Get " + photoPrintsCount + " photoprints: " + (getPhotoPrintsTime.valueOf() - startTime.valueOf()))

        for(var i=0; i < photoPrintsCount; i++)
        {
            var photoPrint = Qt.createQmlObject ('PhotoPrint {}', map)
            newPhotoPrints.push(photoPrint)
        }
        var createTime = new Date()
        console.log("Create photoprints: " + (createTime.valueOf() - startTime.valueOf()))

        for(var i=0; i < photoPrintsCount; i++)
        {
            newPhotoPrints[i].setBorder(photoprintspoints[i*4], photoprintspoints[i*4+1], photoprintspoints[i*4+3], photoprintspoints[i*4+2]);
        }
        var setBorderTime = new Date()
        console.log("Photoprints setBorder: " + (createTime.valueOf() - startTime.valueOf()))

        for(var i=0; i < photoPrintsCount; i++)
        {
            map.addMapItem(newPhotoPrints[i])
        }
        var addedTime = new Date()
        console.log("Added photoprints: " + (addedTime.valueOf() - startTime.valueOf()))

        map.photoPrints = newPhotoPrints
        var drawTime = new Date()
        console.log("Saved photoprints: " + (drawTime.valueOf() - startTime.valueOf()))
    }

    function clearPhotoPrints() {
        var oldPhotoPrints = map.photoPrints;
        var count = oldPhotoPrints.length
        map.photoPrints = [];
        for(var i=0; i<count; i++) {
           var removedPhotoPrint = oldPhotoPrints[i]
           map.removeMapItem(removedPhotoPrint)
        }
    }

    function addTrackMarkers()
    {
        var newTrackMarkers = []
        var trackMarkersPoints = photoPlanner.trackMarkers
        var trackMarkersCount = trackMarkersPoints.length
        for(var i=0; i<trackMarkersCount; i++)
        {
            var trackMarker = Qt.createQmlObject ('TrackMarker {}', map)
            trackMarker.setPosition(trackMarkersPoints[i])
            trackMarker.setNumber(i+1)
            newTrackMarkers.push(trackMarker)
            map.addMapItem(trackMarker)
        }
        map.trackMarkers = newTrackMarkers
    }

    function clearTrackMarkers() {
        var oldTrackMarkers = map.trackMarkers;
        var count = oldTrackMarkers.length
        map.trackMarkers = [];
        for(var i=0; i<count; i++) {
           var removedTrackMarker = oldTrackMarkers[i]
           map.removeMapItem(removedTrackMarker)
        }
    }

    function clearTrack()
    {
        map.removeMapItem(map.track);
    }

/*
    function calculateMarkerRoute()
    {
        routeQuery.clearWaypoints();
        for (var i = currentMarker; i< map.markers.length; i++){
            routeQuery.addWaypoint(markers[i].coordinate)
        }
        routeQuery.travelModes = RouteQuery.PedestrianTravel
        routeQuery.routeOptimizations = RouteQuery.ShortestRoute
        routeQuery.setFeatureWeight(0, 0)
        routeModel.update();
    }
  */
    focus:                  true
    zoomLevel:              (maximumZoomLevel - minimumZoomLevel)/2

    center {
        // The MSQ airport
        latitude:           53.90
        longitude:          28.01
    }

    onCenterChanged:{
        scaleTimer.restart()
        if (map.followme)
            if (map.center !== positionSource.position.coordinate)
                map.followme = false
    }
    onZoomLevelChanged:{
            scaleTimer.restart()
            if (map.followme) map.center = positionSource.position.coordinate
        }
    onWidthChanged:{
        scaleTimer.restart()
    }

    onHeightChanged:{
        scaleTimer.restart()
    }

    Component.onCompleted: {
        markers = new Array();
        mapItems = new Array();
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Plus) {
            map.zoomLevel++
        } else if (event.key === Qt.Key_Minus) {
            map.zoomLevel--
        }
    }

    PositionSource{
        id: positionSource
        active: followme

        onPositionChanged: {
            map.center = positionSource.position.coordinate
        }
    }

    Slider {
         id: zoomSlider;
         visible: !calculationsPanel.visible
         z: map.z + 3
         minimumValue:  map.minimumZoomLevel;
         maximumValue:  map.maximumZoomLevel;
         anchors.margins: 10
         anchors.bottom: scale.top
         anchors.top: parent.top
         anchors.right: parent.right
         orientation : Qt.Vertical
         value: map.zoomLevel
         onValueChanged: {
             map.zoomLevel = value
         }
     }

    Item {
       id: scale
       z: map.z + 3
       visible: !calculationsPanel.visible && scaleText.text != "0 m"
       anchors.bottom: parent.bottom;
       anchors.right: parent.right
       anchors.margins: 20
       height: scaleText.height * 2
       width: scaleImage.width

       Image {
           id: scaleImageLeft
           source: "icons/scale_end.png"
           anchors.bottom: parent.bottom
           anchors.right: scaleImage.left
       }
       Image {
           id: scaleImage
           source: "icons/scale.png"
           anchors.bottom: parent.bottom
           anchors.right: scaleImageRight.left
       }
       Image {
           id: scaleImageRight
           source: "icons/scale_end.png"
           anchors.bottom: parent.bottom
           anchors.right: parent.right
       }
       Label {
           id: scaleText
           color: "#004EAE"
           anchors.centerIn: parent
           text: "0 m"
       }
       Component.onCompleted: {
           map.calculateScale();
       }
   }

    Timer {
        id: scaleTimer
        interval: 100
        running: false
        repeat: false
        onTriggered: {
            map.calculateScale()
        }
    }

    MouseArea {
        id: mouseArea
        property variant lastCoordinate
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true

        onPressed : {
            map.lastX = mouse.x
            map.lastY = mouse.y
            map.pressX = mouse.x
            map.pressY = mouse.y
            lastCoordinate = map.toCoordinate(Qt.point(mouse.x, mouse.y))
        }

        onPositionChanged: {
            if (mouse.button == Qt.LeftButton) {
                map.lastX = mouse.x
                map.lastY = mouse.y
            }

            currX = mouse.x
            currY = mouse.y
        }

        onDoubleClicked: {
            map.center = map.toCoordinate(Qt.point(mouse.x, mouse.y))
            if (mouse.button === Qt.LeftButton) {
                map.zoomLevel++
            } else if (mouse.button === Qt.RightButton) {
                map.zoomLevel--
            }
            lastX = -1
            lastY = -1
        }

        onPressAndHold:{
            if (Math.abs(map.pressX - mouse.x ) < map.jitterThreshold
                    && Math.abs(map.pressY - mouse.y ) < map.jitterThreshold) {
                        showMainMenu(lastCoordinate);
            }
        }
    }

    Dialog {
        id: resultsDlg
//        visible: truem
        modality: Qt.ApplicationModal
        title: "Calculations"
        property alias flightPathLength: flightPathLength
        property alias flightTime:       flightTime
        property alias flightSpeed:      flightSpeed
        property alias picturesCount:    picturesCount
        contentItem: Rectangle {
            color: "gray"
            implicitWidth: 400
            implicitHeight: 100
            GridLayout {
                anchors.fill: parent
                anchors.margins: 5
                columns: 2
                columnSpacing: 10
                rowSpacing: 1
                focus: true
                Text {
                   text: qsTr("Flight length, km")
                }
                Text {
                   id: flightPathLength;
                   text: qsTr("20")
                }
                Text {
                   text: qsTr("Speed, KpH")
                }
                Text {
                   id: flightSpeed
                   text: qsTr("20")
                }
                Text {
                   text: qsTr("Flight Time, min")
                }
                Text {
                   id: flightTime;
                   text: qsTr("20")
                }
                Text {
                   text: qsTr("Pictures")
                }
                Text {
                    id: picturesCount
                   text: qsTr("20")
                }
                Text {
                   text: qsTr("Photo Prints Area, SqKm")
                }
                Text {
                   text: qsTr("20")
                }
            }
       }
    }
}


