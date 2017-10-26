import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2

ToolBar {
    id: mainToolBar
    width:          48
    anchors.top:    parent.top
    anchors.left:   parent.left
    anchors.bottom: parent.bottom

    property alias settings: settings

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
