import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2

Rectangle {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5
        SwipeView {
            id: swipeView
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

            ColumnLayout {
                visible: SwipeView.isCurrentItem
                Layout.fillWidth: true

                Label {
                    Layout.fillWidth:   true
                    Layout.columnSpan:  2
                    text: qsTr("Calculate Photo Plan")
                    horizontalAlignment: Text.AlignHCenter
                }

                RowLayout {
                    spacing: 5
                    Layout.margins: 10

                    Label {
                        Layout.fillWidth:   true
                        horizontalAlignment: "AlignLeft"
                        text: qsTr("Camera Model")
                    }
                    Label {
                        Layout.fillWidth:   true
                        horizontalAlignment: "AlignLeft"
                        text: cbCameraItems.currentText
                    }
                }

                RowLayout {
                    spacing: 5
                    Layout.margins: 10

                    Label {
                        Layout.fillWidth:   true
                        text: qsTr("Lens")
                    }
                    Label {
                        horizontalAlignment: "AlignLeft"
                        text: cbFocusItems.currentText
                    }
                }

                PhotoPlannerParamForm {
                    name: qsTr("Longitudinal overlap, %")
                    from: 0
                    to: 95
                    value: photoPlanner.longitOverlap
                    onValueChanged: { photoPlanner.longitOverlap = value }
                }

                PhotoPlannerParamForm {
                    name: qsTr("Transverse overlap, %")
                    from: 0
                    to: 95
                    value: photoPlanner.transverseOverlap
                    onValueChanged: { photoPlanner.transverseOverlap = value }
                }

                PhotoPlannerParamForm {
                    name: qsTr("Azimuth, \xB0")
                    from: 0
                    to: 359
                    value: photoPlanner.azimuth
                    onValueChanged: { photoPlanner.azimuth = value }
                }

                PhotoPlannerParamForm {
                    name: qsTr("Altitude, m")
                    from: 5
                    to: 5000
                    value: photoPlanner.altitude
                    onValueChanged: { photoPlanner.altitude = value }
                }

                PhotoPlannerParamForm {
                    name: qsTr("GSD, px/sm")
                    from: 1
                    to: 300
                    value: photoPlanner.gsd
                    onValueChanged: { photoPlanner.gsd = value }
                }

                PhotoPlannerParamForm {
                    name: qsTr("Width, m")
                    from: 1
                    to: 1000
                    value: photoPlanner.width
                    onValueChanged: { photoPlanner.width = value }
                }

                PhotoPlannerParamForm {
                    name: qsTr("Max Roll, \xB0")
                    from: 1
                    to: 90
                    value: photoPlanner.maxRoll
                    onValueChanged: { photoPlanner.maxRoll = value }
                }
            }

            ListView {
                id: markersList
                visible: SwipeView.isCurrentItem
                height:200
                Layout.fillWidth: true
                model: map.markers

                delegate: RowLayout{
                    spacing:        1
                    Label {
                        text: 'N: ' + (index+1).toString() + ' '
                        height: 20
                    }
                    Label {
                        text: "Lat:"
                    }
                    TextArea {
                        activeFocusOnPress: true
                        Layout.maximumWidth: 100
                        horizontalAlignment: Text.AlignLeft
                        cursorPosition: 0
             //           wrapMode: TextInput.Wrap

                        text: map.markers[index].coordinate.latitude
                        onEditingFinished: {
                            map.markers[index].coordinate.latitude = text
                            if(map.missionType === "Area")
                            {
                                map.addGeoItem("PolygonItem")
                                map.clearTrack()
                            }
                            else //pathLength
                            {
                                map.addGeoItem("PolylineItem")
                                map.clearTrack()
                            }
                        }
                    }
                    Label {
                        text: "Lon:"
                    }
                    TextArea  {
                        activeFocusOnPress: true
                        Layout.maximumWidth: 100
                        horizontalAlignment: Text.AlignLeft
               //         wrapMode: TextInput.Wrap
                        text: map.markers[index].coordinate.longitude.toString()
                        onEditingFinished: {
                            map.markers[index].coordinate.longitude = text
                            if(map.missionType === "Area")
                            {
                                map.addGeoItem("PolygonItem")
                                map.clearTrack()
                            }
                            else //pathLength
                            {
                                map.addGeoItem("PolylineItem")
                                map.clearTrack()
                            }
                        }
                    }
                    RoundButton {
                        radius: 15
                        Layout.maximumHeight:   15
                        Layout.maximumWidth:    15
                        Image {
                            fillMode: Image.PreserveAspectFit
                            anchors.centerIn: parent
                            sourceSize.height: parent.background.height - 2
                            height: sourceSize.height
                            source: "icons/add.svg"
                        }

                        onClicked: {
                            map.addMarker();
                            console.log(map.markerCounter);
                        }
                    }
                    RoundButton {
                        radius: 15
                        Layout.maximumHeight:   15
                        Layout.maximumWidth:    15
                        Image {
                            fillMode: Image.PreserveAspectFit
                            anchors.centerIn: parent
                            sourceSize.height: parent.radius - 2
                            height: sourceSize.height
                            source: "icons/remove.svg"
                        }
                        onClicked: {
                            map.deleteMarker(index);
                        }
                    }

                }
            }

            GridLayout {
               visible: SwipeView.isCurrentItem
               anchors.margins: 5
               columns: 2
               columnSpacing: 10
               rowSpacing: 5
               focus: true
               Label {
                   Layout.fillWidth:   true
                   Layout.columnSpan:  2
                   text: qsTr("Cameras Parameters")
                   horizontalAlignment: Text.AlignHCenter
               }

               Label {
                   Layout.fillWidth:   true
                   text: qsTr("Camera Model")
               }

               ComboBox {
                   id: cbCameraItems
                   Layout.fillWidth:   true
                   model: ListModel {
                           ListElement { text: "Sony S600";}
                   }
                   Component.onCompleted: {
                       currentIndex = find(photoPlanner.cameraModel);
                   }
                   onCurrentTextChanged: {
                       photoPlanner.cameraModel = currentText;
                   }
               }

               Label {
                   Layout.fillWidth:   true
                   text: qsTr("Focus Range, mm")
               }

               ComboBox {
                   Layout.fillWidth:   true
                   id: cbFocusItems
                   model: ListModel {
                           ListElement { text: "35";}
                           ListElement { text: "50";}
                   }
                   currentIndex: find(photoPlanner.focusRange)
                   onCurrentTextChanged: {
                       photoPlanner.focusRange = currentText;
                   }
               }
            }


            GridLayout {
                visible: SwipeView.isCurrentItem
                anchors.margins: 5
                columns: 2
                columnSpacing: 10
                rowSpacing: 5
                focus: true

                Label {
                    Layout.fillWidth:   true
                    Layout.columnSpan:  2
                    text: qsTr("UAV Parameters")
                    horizontalAlignment: Text.AlignHCenter
                }
                Item {
                    Layout.fillWidth:   true
                    Layout.columnSpan:  2
                    height:             10
                }

                Label {
                    Layout.fillWidth:   true
                    text: qsTr("UAV Model")
                }

                ComboBox {
                    Layout.fillWidth:   true
                    model: ListModel {
                            id: cbUAVModels
                            ListElement { text: "Quadro";}
                            ListElement { text: "Plane 1";}
                            ListElement { text: "Plane 2";}
                    }
                    //currentIndex: find(photoPlanner.cameraModel)
                    onCurrentTextChanged: {
                        //photoPlanner.cameraModel = currentText;
                    }
                }

                Label {
                    Layout.fillWidth:   true
                    text: qsTr("Flight Time, min")
                }

                Slider {
                    Layout.fillWidth:   true
                    from:               1
                    to:                 120
                    value:              40//photoPlanner.transverseOverlap
                    onValueChanged: {
                        //photoPlanner.transverseOverlap = value;
                    }
                    Label {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.top
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignTop
                        text: parent.value.toFixed(0)
                    }
                }

                Label {
                    Layout.fillWidth:   true
                    text: qsTr("Flight Speed, kmph")
                }

                Slider {
                    Layout.fillWidth:   true
                    from:               1
                    to:                 120
                    value:              40//photoPlanner.transverseOverlap
                    onValueChanged: {
                        //photoPlanner.transverseOverlap = value;
                    }
                    Label {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.top
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignTop
                        text: parent.value.toFixed(0)
                    }
                }
                Label {
                    Layout.fillWidth:   true
                    text: qsTr("Comm Radius, km")
                }

                Slider {
                    Layout.fillWidth:   true
                    from:               1
                    to:                 120
                    value:              5//photoPlanner.transverseOverlap
                    onValueChanged: {
                        //photoPlanner.transverseOverlap = value;
                    }
                    Label {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.top
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignTop
                        text: parent.value.toFixed(0)
                    }
                }
                Label {
                    Layout.fillWidth:   true
                    text: qsTr("Speed, KpH")
                }
                Slider {
                    Layout.fillWidth:   true
                    from:                  0
                    to:                 1000
                    value:               photoPlanner.speed
                    onValueChanged: {
                        photoPlanner.speed = value;
                    }
                    Label {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.top
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignTop
                        text: parent.value.toFixed(0)
                    }
                }
            }
        }

      /*  Item {
            Layout.fillHeight: true
        }
*/
        TabBar {
            id: tabBar
            height: 75
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            currentIndex: swipeView.currentIndex
            TabButton {
                text: qsTr("Calc")
            }
            TabButton {
                text: qsTr("Coord")
            }
            TabButton {
                text: qsTr("Cameras")
            }
            TabButton {
                text: qsTr("UAV")
            }
        }
    }
}
