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
        name:   "nokia"
        PluginParameter { name: "here.app_id"; value: "0nq9YLyTnyFO4uiCkHQ4" }
        PluginParameter { name: "here.token"; value: "iRlA2YudpvvSpwzXcrjnPg" }
        PluginParameter { name: "mapping.cache.directory"; value: "/tmp/maps" }
    }


    Map {
        id: map
        anchors.fill: parent

        property MapPolyline linearPoI
        property MapPolygon  areaPoI
        property MapPolyline track


        plugin: mapPlugin
        center: QtPositioning.coordinate(-27.47, 153.025)
        zoomLevel: 14


        function addMarkers()
        {
            var count = photoPlanner.photoCenters.length;

            for(var i=0; i<count; i++)
            {
//                var marker = Qt.createQmlObject ('Marker {}', map)
                //import QtQuick 2.0;
//                property string labelText

                var marker = Qt.createQmlObject ('  import QtLocation 5.6;
                                                  MapCircle {

                                                  }', map)
                /*   Text {
                     anchors.fill: parent
                     text: labelText
                   } */

                map.addMapItem(marker)
                marker.z = map.z
                //marker.coordinate = photoPlanner.photoCenters[i]
                marker.center = photoPlanner.photoCenters[i];
                marker.radius= 20
                marker.border.width = 1;
                marker.border.color =  '#8021be2b' //'red'
                marker.color =  '#2021be2b' // 'transparent'
                //marker.color= 'green'
                //marker. labelText = i;
                //marker.circleText = i;
               // console.log("11");
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
                marker.border.color =  '#8021be2b' //'red'
                marker.color =  '#2021be2b' // 'transparent'

                //marker.circleText = i;
            }
        }

        function startTrack() {
/*
            if(defined === map.track)
            {
                map.removeMapItem(map.track);
            }
  */

            map.removeMapItem(map.track);
            map.track = Qt.createQmlObject('import QtLocation 5.6; MapPolyline {}', item);
            map.track.line.width = 2;
            map.track.line.color = 'blue';
            map.addMapItem(map.track);
            map.track.path = photoPlanner.photoCenters //map.areaPoI.path //photoPlanner.track;
        }

        function startLinearPoI() {

            map.clearMapItems();
            map.linearPoI = Qt.createQmlObject('import QtLocation 5.6; MapPolyline {}', item);
            map.linearPoI.line.width = 2;
            map.linearPoI.line.color = 'green';
            map.addMapItem(map.linearPoI);
        }

        function startAreaPoI() {
            map.clearMapItems();
            map.areaPoI = Qt.createQmlObject('import QtLocation 5.6; MapPolygon {}', item);
            map.areaPoI.color = "#2021be2b";
            map.areaPoI.border.width = 2;
            map.areaPoI.border.color = 'green';
            map.addMapItem(map.areaPoI);
        }

        MouseArea {
            id: mapMouseArea
            anchors.fill: parent
            hoverEnabled: false

            onPressed : {
                if(linearMission.checked)
                {
                    if(map.linearPoI === undefined)
                    {
                        map.startLinearPoI();
                        map.linearPoI.addCoordinate(map.toCoordinate(Qt.point(mouse.x, mouse.y)));
                    }
                    else
                    {
                        map.linearPoI.addCoordinate(map.toCoordinate(Qt.point(mouse.x, mouse.y)));
                    }
                }
                else if (areaMission.checked)
                {
                    if(map.areaPoI===undefined)
                    {
                        map.startAreaPoI();
                        map.areaPoI.addCoordinate(map.toCoordinate(Qt.point(mouse.x, mouse.y)));
                    }
                    else
                    {
                        map.areaPoI.addCoordinate(map.toCoordinate(Qt.point(mouse.x, mouse.y)));
                    }
                }
            }
        }
    }
/*
    Rectangle {
        anchors.fill: parent
  //      width: 200
    //    height:200
        color: "blue"
    }
*/
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
                        photoPlanner.calcLinearPhotoPrints(map.linearPoI.path);

                        map.startTrack();
                        map.addMarkers();
                        map.addPhotoPrints();

                    }

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
                        photoPlanner.calcAreaPhotoPrints(map.areaPoI.path);

                        map.startTrack();
                        map.addMarkers();
                        map.addPhotoPrints();
                        //calculateAoI(map.areaPoI.path);
                    }

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
                ToolTip.delay:          parent.delay
                ToolTip.timeout:        parent.timeout
                ToolTip.visible:        hovered
                ToolTip.text:           qsTr("Saved Missions")
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
                    missionSelectorDialog.open();
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
        id: missionSelectorDialog
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
        color:          "#a021be2b"
    }
}

