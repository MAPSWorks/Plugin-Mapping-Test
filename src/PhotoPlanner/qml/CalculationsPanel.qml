import QtQuick 2.7
//import QtQuick.Controls 1.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2

Rectangle {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5
        SwipeView {
            id: swipeView
            Layout.fillWidth: true
            Layout.fillHeight:  true
            currentIndex: tabBar.currentIndex
            GridLayout {
                visible: swipeView.currentIndex === 0
                columns: 2
                columnSpacing: 10
                rowSpacing: 1
                focus: true

                Label {
                    Layout.fillWidth:   true
                    Layout.columnSpan:  2
                    text: qsTr("Calculate Photo Plan")
                    horizontalAlignment: Text.AlignHCenter
                }

                Item {
                    Layout.fillWidth:   true
                    Layout.columnSpan:  2
                    height:             10
                }

                Label {
                    Layout.fillWidth:   true
                    text: qsTr("Camera Model")
                }

                Label {
                    text: cbCameraItems.currentText
                }

                Label {
                    Layout.fillWidth:   true
                    text: qsTr("Lens")
                }

                Label {
                    text: cbFocusItems.currentText
                }

                Label {
                    Layout.fillWidth:   true
                    text: qsTr("Longitudinal\noverlap, %")
                }

                Slider {
                    Layout.fillWidth:   true
                    from:                  0
                    to:                  100
                    value:              photoPlanner.longitOverlap
                    onValueChanged: {
                        photoPlanner.longitOverlap = value;
                    }
                    Label {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        text: parent.value.toFixed(0)
                    }
                }
                Label {
                    Layout.fillWidth:   true
                    text: qsTr("Transverse\noverlap, %")
                }
                Slider {
                    Layout.fillWidth:   true
                    from:               0
                    to:                 100
                    value:              photoPlanner.transverseOverlap
                    onValueChanged: {
                        photoPlanner.transverseOverlap = value;
                    }
                    Label {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        text: parent.value.toFixed(0)
                    }
                }
                Label {
                    Layout.fillWidth:   true
                    text: qsTr("Azimuth, \xB0")
                }
                Slider {
                     Layout.fillWidth:   true
                     from:                  0
                     to:                  359
                     value:               photoPlanner.azimuth
                     onValueChanged: {
                         photoPlanner.azimuth = value;
                     }
                     Label {
                         anchors.fill: parent
                         horizontalAlignment: Text.AlignHCenter
                         text: parent.value.toFixed(0)
                     }
                }
                Label {
                    Layout.fillWidth:   true
                    text: qsTr("Altitude, m")

                }
                Slider {
                    Layout.fillWidth:   true
                    from:                  0
                    to:                 5000
                    value:               photoPlanner.altitude
                    onValueChanged: {
                        photoPlanner.altitude = value;
                    }
                    Label {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        text: parent.value.toFixed(0)
                    }
                }
                Label {
                    Layout.fillWidth:   true
                    text: qsTr("GSD, px/sm")
                }
                Slider {
                       Layout.fillWidth:   true
                       from:                  1
                       to:                  300
                       value:               photoPlanner.gsd
                       onValueChanged: {
                           photoPlanner.gsd = value;
                       }
                       Label {
                           anchors.fill: parent
                           horizontalAlignment: Text.AlignHCenter
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
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        text: parent.value.toFixed(0)
                    }
                }
                Label {
                    Layout.fillWidth:   true
                    text: qsTr("Whidth, m")
                }
                Slider {
                    Layout.fillWidth:   true
                    from:                  0
                    to:                 1000
                    value:               photoPlanner.width
                    onValueChanged: {
                        photoPlanner.width = value;
                    }
                    Label {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        text: parent.value.toFixed(0)
                    }
                }
                Label {
                    Layout.fillWidth:   true
                    text: qsTr("Max Roll, \xB0")
                }
                Slider {
                    Layout.fillWidth:   true
                    from:                  0
                    to:                   90
                    value:               photoPlanner.maxRoll
                    onValueChanged: {
                        photoPlanner.maxRoll = value;
                    }
                    Label {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        text: parent.value.toFixed(0)
                    }
                }
            }

            ListView {
                id: markersList
                height:200
                Layout.fillWidth: true
                model: map.markers

                delegate: RowLayout{
                    spacing:        1
                    Label {
                        text: index
                        height: 20
                    }
                    Label {
                        text: "Lat:"
                    }
                    TextField {
                        activeFocusOnPress: true
                        Layout.maximumWidth: 100
                        wrapMode: TextInput.Wrap
                        text: map.markers[index].coordinate.latitude.toString()
                        onEditingFinished: {
                            map.markers[index].coordinate.latitude = text
                        }
                    }
                    Label {
                        text: "Lon:"
                    }
                    TextField {
                        activeFocusOnPress: true
                        Layout.maximumWidth: 100
                        wrapMode: TextInput.Wrap
                        text: map.markers[index].coordinate.longitude.toString()
                        onEditingFinished: {
                            map.markers[index].coordinate.longitude = text
                        }
                    }
                    Button {
                        text:"1"
                        Layout.maximumWidth: 20
                        onClicked: {
                            map.addMarker();
                            console.log(map.markerCounter);
                        }
                    }
                    Button {
                        Layout.maximumWidth: 20
                        text:"2"
                        onClicked: {
                            map.deleteMarker(index);
                        }
                    }

                }
                        /*
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                map.addMarker();
                            }
                        }*/
                    }
            /*
            Rectangle {
                visible: swipeView.currentIndex === 1
                color: "red"
                height: 100
                width:  200
            }

            /* GridLayout {
                visible: swipeView.currentIndex === 1
                columns: 1
                columnSpacing: 10
                rowSpacing: 1
                focus: true
                Rectangle {
                  //  Layout.fillHeight: true
                    Layout.fillWidth: true
                    height: swipeView.height
                  //  width: 200
                    color: "red"
                }
*/

            /*
                TableView {
                    id: tableView
                    frameVisible: true
                    sortIndicatorVisible: false
                    height:swipeView.height
                    Layout.fillHeight: true
                    Layout.fillWidth: true
               //     anchors.fill: parent
                    visible: swipeView.currentIndex === 1

                    TableViewColumn {
                               id: numberColumn
                               title: "#"
                               role: "num"
                               movable: false
                               resizable: false
                               width: 20
                           }

                    TableViewColumn {
                               id: latitudeColumn
                               title: "latitude"
                               role: "latitude"
                               movable: false
                               resizable: false
                               width: 100
                           }

                    TableViewColumn {
                               id: longitColumn
                               title: "longitude"
                               role: "longitude"
                               movable: false
                               resizable: false
                               width: 100
                           }

                    ListModel {
                               id: sourceModel
                               ListElement {
                                   num: 1
                                   latitude: 38.7
                                   longitude: 29.5
                               }
                               ListElement {
                                   num: 2
                                   latitude: 38.7
                                   longitude: 29.5
                               }
                               ListElement {
                                   num: 3
                                   latitude: 38.7
                                   longitude: 29.5
                               }
                    }
                    model: sourceModel
                    itemDelegate: Item{
                        Text {
                        text: styleData.value
                        }
                    }
                }
*/
  //          }
            GridLayout {
               visible: swipeView.currentIndex === 2
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
                   currentIndex: find(photoPlanner.cameraModel)
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
                visible: swipeView.currentIndex === 3
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
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
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
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
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
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
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
            height: 50
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
