import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2
import QtLocation 5.9
import QtPositioning 5.8
import QtQuick.Controls.Styles 1.4

Item {
    id:item
    Plugin {
        id:     mapPlugin
        name:  "mapbox" //"osm" // "mapbox" //"nokia"

        PluginParameter { name: "mapbox.access_token"; value:"pk.eyJ1IjoiZ2Zyb2wiLCJhIjoiY2o4NXlidWNpMGxocTMyc2I4N2diZjltNSJ9.Syw1X8CQ2nR_7cYOqIFkfA" }
        //PluginParameter { name: "mapboxgl.access_token"; value:"pk.eyJ1IjoiZ2Zyb2wiLCJhIjoiY2o4NXlidWNpMGxocTMyc2I4N2diZjltNSJ9.Syw1X8CQ2nR_7cYOqIFkfA" }
        PluginParameter { name: "mapbox.mapping.cache.directory"; value: "/tmp/maps" }
        PluginParameter { name: "style"; value: "mapbox://styles/mapbox/satellite-v9" }
    }

    Image {
        id: poiImage
        source: "icons/settings.svg"
    }

    MapQuickItem {
        id: poiMarker
        sourceItem: poiImage
    }

    PlannerMap {
        id:             map
        anchors.fill:   parent
        plugin:         mapPlugin
        zoomLevel:      14

        onShowMainMenu: {
            if(toolBar.linearMission.checked || toolBar.areaMission.checked)
            {
                map.addMarker()
            }
//          mapPopupMenu.show(coordinate);
        }
        onShowMarkerMenu: {
//          markerPopupMenu.show(coordinate);
        }
    }

    MapPopupMenu {
        id: mapPopupMenu
        function show(coordinate)
        {
            mapPopupMenu.coordinate = coordinate
            mapPopupMenu.markersCount = map.markers.length
            mapPopupMenu.mapItemsCount = map.mapItems.length
            mapPopupMenu.update()
            mapPopupMenu.popup()
        }
        onItemClicked: {
            switch (item) {
                case "addMarker":
                    map.addMarker()
                    break
//                case "getCoordinate":
//                    map.coordinatesCaptured(coordinate.latitude, coordinate.longitude)
//                    break
                case "fitViewport":
                    map.fitViewportToMapItems()
                    break
                case "deleteMarkers":
                    map.deleteMarkers()
                    break
                case "deleteItems":
                    map.deleteMapItems()
                    break
                default:
                    console.log("Unsupported operation")
            }
        }
    }

    MarkerPopupMenu {
        id: markerPopupMenu
        function show(coordinate)
        {
           // stackView.pop(page)
            markerPopupMenu.markersCount = map.markers.length
            markerPopupMenu.update()
            markerPopupMenu.popup()
        }
        function moveMarker(coordinate)
        {
            map.markers[map.currentMarker].coordinate = coordinate;
            map.center = coordinate;
           // stackView.pop(page)
        }

        onItemClicked: {
        //    stackView.pop(page)
            switch (item) {
                case "deleteMarker":
                    map.deleteMarker(map.currentMarker)
                    break;
                case "getMarkerCoordinate":
                    map.coordinatesCaptured(map.markers[map.currentMarker].coordinate.latitude, map.markers[map.currentMarker].coordinate.longitude)
                    break;
                case "routeToNextPoint":
                case "routeToNextPoints":
                    map.calculateMarkerRoute()
                    break
                case "distanceToNextPoint":
                    var coordinate1 = map.markers[currentMarker].coordinate;
                    var coordinate2 = map.markers[currentMarker+1].coordinate;
                    var distance = Helper.formatDistance(coordinate1.distanceTo(coordinate2));
                    stackView.showMessage(qsTr("Distance"),"<b>" + qsTr("Distance:") + "</b> " + distance)
                    break
                case "drawPolyline":
                    map.addGeoItem("PolylineItem")
                    break;
                case "drawPolygonMenu":
                    map.addGeoItem("PolygonItem")
                    break
                default:
                    console.log("Unsupported operation")
            }
        }
    }

/*
    Map {
        id: map
        anchors.fill: parent

        property MapPolyline linearPoI
        property MapPolygon  areaPoI
        property MapPolyline track
        property list<MapQuickItem> trackMarkers

        property string missionType

        plugin: mapPlugin
        center: QtPositioning.coordinate(53.90  , 28.01)
        zoomLevel: 14


        function addTrackMarkers()
        {
            var count = photoPlanner.trackMarkers.length // photoCenters.length;
            for(var i=0; i<count; i++)
            {
                addMarkerTrack(photoPlanner.trackMarkers[i], i+1);
//                var marker = Qt.createQmlObject ('  import QtQuick 2.5;
//                                                    import QtLocation 5.6;
//                                                    //property var itm : "111";
//                                                    MapQuickItem {
//                                                    sourceItem: Image {
//                                                        fillMode: Image.PreserveAspectFit
//                                                        anchors.centerIn: parent
//                                                        sourceSize.height: 14
//                                                        height: sourceSize.height
//                                                        source: "icons/settings.svg"
//                                                        Text {
//                                                            id: markerText
//                                                            text: "'+i+'"
//                                                            x:10
//                                                            y:10
//                                                            //anchors.centerIn: parent
//                                                        }
//                                                        MouseArea {
//                                                            anchors.fill: parent
//                                                            onPressed {
//                                                                console.log(markerText)
//                                                            }
//                                                        }
//                                                    }

//                                                  }', map);

//                map.addMapItem(marker)
//                marker.z = map.z-1
//                marker.coordinate = photoPlanner.photoCenters[i];
            }
        }

        function addPhotoPrints()
        {
            var count = photoPlanner.photoPrints.length/4;

            for(var i=0; i<count; i++)
            {
//                var marker = Qt.createQmlObject ('Marker {}', map)
                var marker = Qt.createQmlObject ('import QtLocation 5.6; MapPolygon {}', map)
                map.addMapItem(marker)
                marker.z = map.z
                //marker.coordinate = photoPlanner.photoCenters[i]
                marker.addCoordinate(photoPlanner.photoPrints[i*4]);
                marker.addCoordinate(photoPlanner.photoPrints[i*4+1]);
                marker.addCoordinate(photoPlanner.photoPrints[i*4+3]);
                marker.addCoordinate(photoPlanner.photoPrints[i*4+2]);
//                marker.radius= 20
                marker.border.width= 1
                marker.border.color =  '#80cc9933' //'red'
                marker.color =  '#20cc9933'//'#2021be2b' // 'transparent'

                //marker.circleText = i;
            }
        }

        function startTrack() {
            map.removeMapItem(map.track);
            for(var i = 0; i<map.trackMarkers.length; i++)
                map.removeMapItem(map.trackMarkers[i])
            map.trackMarkers = null
            map.track = Qt.createQmlObject('import QtLocation 5.6; MapPolyline {}', item);
            map.track.line.width = 2;
            map.track.line.color = 'blue';
            map.addMapItem(map.track);
            map.track.path = photoPlanner.track; //map.areaPoI.path //photoPlanner.track;
        }

        function startLinearPoI() {

            map.clearMapItems();
            map.linearPoI = Qt.createQmlObject('import QtLocation 5.6; MapPolyline {}', item);
            map.linearPoI.line.width = 2;
            map.linearPoI.line.color = 'green';
            map.addMapItem(map.linearPoI);
            map.missionType="Linear";
        }

        function startAreaPoI() {
            map.clearMapItems();
            map.areaPoI = Qt.createQmlObject('import QtLocation 5.6; MapPolygon {}', item);
            map.areaPoI.color = "#2021be2b";
            map.areaPoI.border.width = 2;
            map.areaPoI.border.color = 'green';
            map.addMapItem(map.areaPoI);
            map.missionType="Area";
        }

        function calculatePhotoPlan () {
            if(missionType === "Area")
            {
                photoPlanner.calcAreaPhotoPrints(map.areaPoI.path);
            }
            else //pathLength
            {
                photoPlanner.calcLinearPhotoPrints(map.linearPoI.path);
            }
            startTrack();
            addTrackMarkers();
            addPhotoPrints();
        }


        function addMarker(coordinate, number, image) {
            var marker = Qt.createQmlObject ('  import QtQuick 2.5;
                                                import QtLocation 5.6;
                                                //property var itm : "111";
                                                MapQuickItem {
                                                sourceItem: Image {
                                                    fillMode: Image.PreserveAspectFit
                                                    anchors.centerIn: parent
                                                    sourceSize.height: 20
                                                    height: sourceSize.height
                                                    source: "' + image + '"
                                                    Text {
                                                        id: txt
                                                        text: "'+number+'"
                                                        x:20
                                                        y:20
                                                        //anchors.centerIn: parent
                                                    }
                                                MouseArea {
                                                            anchors.fill: parent
                                                            acceptedButtons: Qt.LeftButton
                                                            onClicked: {
                                                                console.log("markerText")
                                                                txt.text = "wwww";
                                                            }
                                                        }
                                                }
                                              }', map);
            marker.coordinate = coordinate
            map.addMapItem(marker)
            return marker
        }

        function addMarkerPoI(coordinate, number) {
            addMarker(coordinate, number, "icons/place.svg")
        }
        function addMarkerTrack(coordinate, number) {
            var marker = addMarker(coordinate, number, "icons/place.svg")
            trackMarkers.push(marker)
        }

        MouseArea {
            id: mapMouseArea
            anchors.fill: parent
            hoverEnabled: true
       //     enabled: linearMission.checked || areaMission.checked

            onPositionChanged: {
                coordLabel.text = map.toCoordinate(Qt.point(mouseX, mouseY)).toString() //mouseX //map.toCoordinate(Qt.point(mouseX, mouseY)).x
            }

            onPressed : {
                var mouseCoordinate = map.toCoordinate(Qt.point(mouse.x, mouse.y))

                if(linearMission.checked && (map.missionType==="Linear"))
                {
                    if(map.linearPoI === undefined)
                    {
                        map.startLinearPoI();
                    }
                    map.linearPoI.addCoordinate(mouseCoordinate);
                    map.addMarkerPoI(mouseCoordinate, map.linearPoI.path.length)
                }
                else if (areaMission.checked && (map.missionType==="Area"))
                {
                    if(map.areaPoI===undefined)
                    {
                        map.startAreaPoI();
                    }
                    map.areaPoI.addCoordinate(map.toCoordinate(Qt.point(mouse.x, mouse.y)));
                    map.addMarkerPoI(mouseCoordinate, map.areaPoI.path.length)
                }
            }
        }
    }
*/


    Label {
        id: coordLabel
        anchors.left:   toolBar.right
        anchors.bottom: parent.bottom
        anchors.margins: 5
        color:          "#aa21be2b"
        text: map.toCoordinate(Qt.point(map.currX, map.currY)).toString() //mouseX //map.toCoordinate(Qt.point(mouseX, mouseY)).x
    }

    PlannerToolBar {
        id: toolBar
    }

    Text {
        anchors.top: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10
        text: "Photo Planner"
    }

    FileDialog {
        id: aoiSaveDialog
        title: "Enter filename to save photo region"
        selectExisting: false
        selectMultiple: false
        selectFolder: false
        modality: Qt.WindowModal
        nameFilters: [ "Aoi files (*.aoi)", "All files (*)" ]
        selectedNameFilter: "Aoi files (*.aoi)"

        Component.onCompleted: {
            folder = shortcuts.home + "/.gcu/flightplans";
        }

        onAccepted: {
            var myArray = new Array()
            for (var i = 0; i<map.markerCounter; i++){
                myArray.push(map.markers[i].coordinate)
            }
            photoPlanner.saveAoi(fileUrl, map.missionType, myArray)
        }
    }

    FileDialog {
        id: aoiLoadDialog
        title: "Enter filename to load photo region"
        selectExisting: true
        selectMultiple: false
        selectFolder: false
        modality: Qt.WindowModal
        nameFilters: [ "Aoi files (*.aoi)", "All files (*)" ]
        selectedNameFilter: "Aoi files (*.aoi)"

        Component.onCompleted: {
            folder = shortcuts.home + "/.gcu/flightplans";
        }

        onAccepted: {
            var loadedAoi = photoPlanner.loadAoi(fileUrl)

            map.clearTrack()
            map.clearMapItems();
            map.markers = []
            map.markerCounter = 0
            map.missionType = photoPlanner.missionType
            for (var i = 0; i < loadedAoi.length; i++) {
                 map.addCoordinateMarker(loadedAoi[i]);
            }
        }
    }

    FileDialog {
        id: saveFlightPlanDialog
        title: "Enter filename to save generated flight plan:"
        selectExisting: false
        selectMultiple: false
        selectFolder: false
        modality: Qt.WindowModal
        nameFilters: [ "Xml files (*.xml)", "All files (*)" ]
        selectedNameFilter: "Xml files (*.xml)"

        Component.onCompleted: {
            folder = shortcuts.home + "/.gcu/flightplans";
        }

        onAccepted: {
            photoPlanner.saveFlightPlan(fileUrl);
        }
    }

    CalculationsPanel {
        id:             calculationsPanel
        visible:        toolBar.settings.checked
        width:          350
        anchors.top:    item.top
        anchors.bottom: item.bottom
        anchors.right:  item.right
        color:          "#2021be2b"
    }
}

