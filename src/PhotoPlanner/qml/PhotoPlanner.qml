import QtQuick 2.0
import QtQuick.Controls 2.0
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


     //   PluginParameter { name: "here.app_id"; value: "0nq9YLyTnyFO4uiCkHQ4" }
       // PluginParameter { name: "here.token"; value: "iRlA2YudpvvSpwzXcrjnPg" }
        //PluginParameter { name: "mapping.cache.directory"; value: "/tmp/maps" }
     //   preferred: ["here", "osm"]
       // required: Plugin.AnyMappingFeatures
    }

    Image {
        id: poiImage
        source: "icons/settings.svg"
    }
    MapQuickItem {
        id: poiMarker
        sourceItem: poiImage
    }

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
                                                        text: "'+number+'"
                                                        x:20
                                                        y:20
                                                        //anchors.centerIn: parent
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

    Label {
        id: coordLabel
        anchors.left:   mainToolBar.right
        anchors.bottom: parent.bottom
        anchors.margins: 5
        color:          "#aa21be2b"
        text: ""
    }

    ToolBar {
        id: mainToolBar
        width:          48
        anchors.top:    parent.top
        anchors.left:   parent.left
        anchors.bottom: parent.bottom

        background: Rectangle {
           implicitWidth: 40
           color: "#2021be2b"
        }


        ColumnLayout {
            id:                             topToolBarLayout
            anchors.fill:                   parent
            focus:                          false
            spacing:                        2
            property bool bHoverEnabled:    true
            property int  delay:            1000
            property int  timeout:          5000

            ToolButton {
                id:                     newMission
                hoverEnabled:           parent.bHoverEnabled
                checkable:              true
                checked:                false
                ToolTip.delay:          parent.delay
                ToolTip.timeout:        parent.timeout
                ToolTip.visible:        hovered
                ToolTip.text:           qsTr("New Mission")
                Rectangle {
                    anchors.centerIn: parent
                    height: parent.background.height*1.5
                    width:  parent.background.height*1.5
                    radius: parent.background.height*1.5
                    color: parent.checked? "#6021be2b" : "transparent"
                    border.width: 1
                    border.color: parent.hovered? "#F021be2b" : "#6021be2b"
                    visible: parent.hovered || parent.checked
                }
                Image {
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                    sourceSize.height: parent.background.height - 2
                    height: sourceSize.height
                    source: "icons/map.svg"
                }
                onClicked: {
                    //newMissionPopup.x = mouseX
                    //newMissionPopup.y = mouseY
                    //newMissionPopup.open()
               //       map.start();
                }

            }

            ToolButton {
                id:                     linearMission
                hoverEnabled:           parent.bHoverEnabled
                checkable:              true
                checked:                false
                visible:                newMission.checked
                ToolTip.delay:          parent.delay
                ToolTip.timeout:        parent.timeout
                ToolTip.visible:        hovered
                ToolTip.text:           qsTr("Linear Mission")
                Rectangle {
                    anchors.centerIn: parent
                    height: parent.background.height*1.5
                    width:  parent.background.height*1.5
                    radius: parent.background.height*1.5
                    color: parent.checked? "#6021be2b" : "transparent"
                    border.width: 1
                    border.color: parent.hovered? "#F021be2b" : "#6021be2b"
                    visible: parent.hovered || parent.checked
                }
                Image {
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                    sourceSize.height: parent.background.height - 2
                    height: sourceSize.height
                    source: "icons/linear.svg"
                }
                onClicked: {
                    if(checked)
                    {
                        map.startLinearPoI();
                    }
                    else
                    {
          /*              photoPlanner.calcLinearPhotoPrints(map.linearPoI.path);

                        map.addPhotoPrints();
                        map.addMarkers();
                        map.startTrack();
*/                    }

                    //newMissionPopup.x = mouseX
                    //newMissionPopup.y = mouseY
                    //newMissionPopup.open()
               //       map.start();
                }

            }

            ToolButton {
                id:                     areaMission
                hoverEnabled:           parent.bHoverEnabled
                checkable:              true
                checked:                false
                visible:                newMission.checked
                ToolTip.delay:          parent.delay
                ToolTip.timeout:        parent.timeout
                ToolTip.visible:        hovered
                ToolTip.text:           qsTr("Area Mission")
                Rectangle {
                    anchors.centerIn: parent
                    height: parent.background.height*1.5
                    width:  parent.background.height*1.5
                    radius: parent.background.height*1.5
                    color: parent.checked? "#6021be2b" : "transparent"
                    border.width: 1
                    border.color: parent.hovered? "#F021be2b" : "#6021be2b"
                    visible: parent.hovered || parent.checked
                }
                Image {
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                    sourceSize.height: parent.background.height - 2
                    height: sourceSize.height
                    source: "icons/area.svg"
                }
                onClicked: {

                    if(checked)
                    {
                        map.startAreaPoI();
                    }
                    else
                    {
              //          photoPlannerWindow.aoi = map.areaPoI.path;
  /*                      photoPlanner.calcAreaPhotoPrints(map.areaPoI.path);

                        map.startTrack();
                        map.addMarkers();
                        map.addPhotoPrints();
                        //calculateAoI(map.areaPoI.path);
    */                }

/*                    else
                    {
                        if(map.track.coordinateAt(map.track.pathLength()-1)!=map.track.coordinateAt(0))
                            map.track.addCoordinate(map.track.coordinateAt(0));
                    }
*/
                    //newMissionPopup.x = mouseX
                    //newMissionPopup.y = mouseY
//                    newMissionPopup.open()
                    // map.start();
                }

            }

            ToolButton {
                hoverEnabled:           parent.bHoverEnabled
                visible:                newMission.checked
                checkable:              false
                checked:                false
                ToolTip.delay:          parent.delay
                ToolTip.timeout:        parent.timeout
                ToolTip.visible:        hovered
                ToolTip.text:           qsTr("Calculate")
                Rectangle {
                    anchors.centerIn: parent
                    height: parent.background.height*1.5
                    width:  parent.background.height*1.5
                    color: "#6021be2b"
                    radius: parent.background.height*1.5
                    visible: parent.hovered
                }
                Image {
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                    sourceSize.height: parent.background.height - 2
                    height: sourceSize.height
                    source: "icons/camera.svg"
                }
                onClicked: {
                    map.calculatePhotoPlan();
                }
            }

            ToolButton {
                hoverEnabled:           parent.bHoverEnabled
                ToolTip.delay:          parent.delay
                ToolTip.timeout:        parent.timeout
                ToolTip.visible:        hovered
                ToolTip.text:           qsTr("Load photo region ...")
                Rectangle {
                    anchors.centerIn: parent
                    height: parent.background.height*1.5
                    width:  parent.background.height*1.5
                    color: "#6021be2b"
                    radius: parent.background.height*1.5
                    visible: parent.hovered
                }
                Image {
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                    sourceSize.height: parent.background.height - 2
                    height: sourceSize.height
                    source: "icons/unarchive.svg"
                }
                onClicked: {
                    aoiLoadDialog.open();
                }
            }
            ToolButton {
                hoverEnabled:           parent.bHoverEnabled
                ToolTip.delay:          parent.delay
                ToolTip.timeout:        parent.timeout
                ToolTip.visible:        hovered
                ToolTip.text:           qsTr("Save photo region ...")
                Rectangle {
                    anchors.centerIn: parent
                    height: parent.background.height*1.5
                    width:  parent.background.height*1.5
                    color: "#6021be2b"
                    radius: parent.background.height*1.5
                    visible: parent.hovered
                }
                Image {
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                    sourceSize.height: parent.background.height - 2
                    height: sourceSize.height
                    source: "icons/archive.svg"
                }
                onClicked: {
                    aoiSaveDialog.open();
                }
            }

            ToolButton {
                hoverEnabled:           parent.bHoverEnabled
                ToolTip.delay:          parent.delay
                ToolTip.timeout:        parent.timeout
                ToolTip.visible:        hovered
                ToolTip.text:           qsTr("Save generated flight plan")
                Rectangle {
                    anchors.centerIn: parent
                    height: parent.background.height*1.5
                    width:  parent.background.height*1.5
                    color: "#6021be2b"
                    radius: parent.background.height*1.5
                    visible: parent.hovered
                }
                Image {
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                    sourceSize.height: parent.background.height - 2
                    height: sourceSize.height
                    source: "icons/archive.svg"
                }
                onClicked: {
                    saveFlightPlanDialog.open()

                }
            }

            ToolButton {
                id:                     settings
                checkable:              true
                checked:                false
                hoverEnabled:           parent.bHoverEnabled
                ToolTip.delay:          parent.delay
                ToolTip.timeout:        parent.timeout
                ToolTip.visible:        hovered
                ToolTip.text:           qsTr("Settings")
                Rectangle {
                    anchors.centerIn: parent
                    height: parent.background.height*1.5
                    width:  parent.background.height*1.5
                    radius: parent.background.height*1.5
                    color: parent.checked? "#6021be2b" : "transparent"
                    border.width: 1
                    border.color: parent.hovered? "#F021be2b" : "#6021be2b"
                    visible: parent.hovered || parent.checked
                }
                Image {
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                    sourceSize.height: parent.background.height - 2
                    height: sourceSize.height
                    source: "icons/settings.svg"
                }
                onClicked: {
                    console.log("settings")
                }
            }
            Item {
                Layout.fillHeight: true
            }
        }
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
        folder: shortcuts.home + "/.gcu/flightplans"
        modality: Qt.WindowModal
        nameFilters: [ "Aoi files (*.aoi)", "All files (*)" ]
        selectedNameFilter: "Aoi files (*.aoi)"
        onAccepted: {
            photoPlanner.saveAoi(fileUrl, map.missionType, map.linearPoI.path)
        }
    }

    FileDialog {
        id: aoiLoadDialog
        title: "Enter filename to load photo region"
        selectExisting: true
        selectMultiple: false
        selectFolder: false
        folder: shortcuts.home + "/.gcu/flightplans"
        modality: Qt.WindowModal
        nameFilters: [ "Aoi files (*.aoi)", "All files (*)" ]
        selectedNameFilter: "Aoi files (*.aoi)"
        onAccepted: {
            var loadedAoi = photoPlanner.loadAoi(fileUrl)
            map.missionType = photoPlanner.missionType
            if (map.missionType === "Linear") {
                map.startLinearPoI()
                map.linearPoI.path = loadedAoi
            }
            else {
                map.startAreaPoI()
                map.areaPoI.path = loadedAoi;
            }
        }
    }

    FileDialog {
        id: saveFlightPlanDialog
        title: "Enter filename to save generated flight plan:"
        selectExisting: false
        selectMultiple: false
        selectFolder: false
        folder: shortcuts.home + "/.gcu/flightplans"
        modality: Qt.WindowModal
        nameFilters: [ "Xml files (*.xml)", "All files (*)" ]
        selectedNameFilter: "Xml files (*.xml)"
        onAccepted: {
            photoPlanner.saveFlightPlan(fileUrl);
        }
    }

    Menu {
        id: newMissionPopup
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        //anchors.left: mainToolBar.right
        x: mainToolBar.width
        y: 10
        ColumnLayout {
             anchors.fill: parent
             MenuItem {
                 id: addFlightItem;
                 text: qsTr("Linear object");
                 onClicked: {
                    console.log("Add");
                    // flightsTableView.model.append({lineNo: "pmpt", participant:"LINEDIT"})
                    newMissionPopup.close();
                    //settingsDialog.open();
                     //calculationDialog.open();
                     //settingsDialog.visible = true
                     calculationsPanel.visible = true;

                 }
             }
             MenuItem {
                 id: removeFlightItem;
                 text: qsTr("Area object");
                 onClicked: {
//                     flightsTableView.model.remove(flightsTableView.currentRow);
                     console.log("Remove");
                     newMissionPopup.close();
                     calcParamPopup.open()
                 }
             }
        }
    }

    Menu {
        id: calcParamPopup
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        //anchors.left: mainToolBar.right
        x: mainToolBar.width
        y: 10
        height: loadItem.height*8
        ColumnLayout {
             anchors.fill: parent
             MenuItem {
                 id: loadItem
                 text: qsTr("Load");
                 onClicked: {
                    console.log("Add");
                    // flightsTableView.model.append({lineNo: "pmpt", participant:"LINEDIT"})
                    calcParamPopup.close();
                 }
             }
             MenuItem {
                 text: qsTr("Calculate");
                 onClicked: {
//                     flightsTableView.model.remove(flightsTableView.currentRow);
                     console.log("Remove");
                     calcParamPopup.close();
                 }
             }
             MenuItem {
                 text: qsTr("Open File");
                 onClicked: {
//                     flightsTableView.model.remove(flightsTableView.currentRow);
                     console.log("Remove");
                     calcParamPopup.close();
                 }
             }
             MenuItem {
                 text: qsTr("Save File");
                 onClicked: {
//                     flightsTableView.model.remove(flightsTableView.currentRow);
                     console.log("Remove");
                     calcParamPopup.close();
                 }
             }
             MenuItem {
                 text: qsTr("Reverse");
                 onClicked: {
//                     flightsTableView.model.remove(flightsTableView.currentRow);
                     console.log("Remove");
                     calcParamPopup.close();
                 }
             }
             MenuItem {
                 text: qsTr("Import kml");
                 onClicked: {
//                     flightsTableView.model.remove(flightsTableView.currentRow);
                     console.log("Import kml");
                     calcParamPopup.close();
                 }
             }
             MenuItem {
                 text: qsTr("Export kml");
                 onClicked: {
//                     flightsTableView.model.remove(flightsTableView.currentRow);
                     console.log("Export kml");
                     calcParamPopup.close();
                 }
             }
        }
    }


    CalculationsPanel {
        id:             calculationsPanel
        visible:        settings.checked
        width:          350
        anchors.top:    item.top
        anchors.bottom: item.bottom
        anchors.right:  item.right
        color:          "#a0212121"
    }
}

