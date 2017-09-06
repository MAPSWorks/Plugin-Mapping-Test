import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2

Rectangle {

    GridLayout {
        anchors.fill: parent
        anchors.margins: 5
        columns: 2
        columnSpacing: 10
        rowSpacing: 5
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

        ComboBox {
            Layout.fillWidth:   true
            model: ListModel {
                    id: cbCameraItems
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
            model: ListModel {
                    id: cbFocusItems
                    ListElement { text: "35";}
                    ListElement { text: "50";}
            }
            currentIndex: find(photoPlanner.focusRange)
            onCurrentTextChanged: {
                photoPlanner.focusRange = currentText;
            }
        }

        Label {
            Layout.fillWidth:   true
            text: qsTr("Longitudinal\noverlap, %")
        }

        SpinBox {
            Layout.fillWidth:   true
            from:                  0
            to:                  100
            value:              photoPlanner.longitOverlap
            onValueChanged: {
                photoPlanner.longitOverlap = value;
            }
        }
        Label {
            Layout.fillWidth:   true
            text: qsTr("Transverse\noverlap, %")
        }
        SpinBox {
            Layout.fillWidth:   true
            from:               0
            to:                 100
            value:              photoPlanner.transverseOverlap
            onValueChanged: {
                photoPlanner.transverseOverlap = value;
            }
        }
        Label {
            Layout.fillWidth:   true
            text: qsTr("Azimuth, \xB0")
        }
        SpinBox {
            Layout.fillWidth:   true
            from:                  0
            to:                  359
            value:               photoPlanner.azimuth
            onValueChanged: {
                photoPlanner.azimuth = value;
            }
        }
        Label {
            Layout.fillWidth:   true
            text: qsTr("Altitude, m")

        }
        SpinBox {
            Layout.fillWidth:   true
            from:                  0
            to:                 5000
            value:               photoPlanner.altitude
            onValueChanged: {
                photoPlanner.altitude = value;
            }
        }
        Label {
            Layout.fillWidth:   true
            text: qsTr("GSD, px/sm")
        }
        SpinBox {
            Layout.fillWidth:   true
            from:                  1
            to:                  300
            value:               photoPlanner.gsd
            onValueChanged: {
                photoPlanner.gsd = value;
            }
        }

        Label {
            Layout.fillWidth:   true
            text: qsTr("Speed, KpH")
        }
        SpinBox {
            Layout.fillWidth:   true
            from:                  0
            to:                 1000
            value:               photoPlanner.speed
            onValueChanged: {
                photoPlanner.speed = value;
            }
        }
        Label {
            Layout.fillWidth:   true
            text: qsTr("Whidth, m")
        }
        SpinBox {
            Layout.fillWidth:   true
            from:                  0
            to:                 1000
            value:               photoPlanner.width
            onValueChanged: {
                photoPlanner.width = value;
            }
        }
        Label {
            Layout.fillWidth:   true
            text: qsTr("Max Roll, \xB0")
        }
        SpinBox {
            Layout.fillWidth:   true
            from:                  0
            to:                  360
            value:               photoPlanner.maxRoll
            onValueChanged: {
                photoPlanner.maxRoll = value;
            }
        }
        Item {
            Layout.fillHeight: true
        }

        Button {
            Layout.fillWidth:   true
            Layout.columnSpan:  2
            text:               qsTr("Calculate")
            onClicked: {
     //           photoPlannerWindow.calculateAoI(map.areaPoI.path);
            }
        }
    }
}
