import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2

Rectangle {
    id: photoPlannerParamsView

    signal calculateGsd()
    signal calculateAltitude()

    property bool isInitialized: false
    Component.onCompleted: {
        isInitialized = true
        calculateGsd()
    }

    property bool isCalculateGsd: false
    onCalculateGsd: {
        if (isInitialized && !isCalculateAltitude) {
            isCalculateGsd = true
            paramCalcGsd.value = (photoPlanner.calcPhotoParamGsdOnAltitude() * 100).toFixed(2)
            isCalculateGsd = false
        }
    }
    property bool isCalculateAltitude: false
    onCalculateAltitude: {
        if (isInitialized && !isCalculateGsd) {
            isCalculateAltitude = true
            paramCalcAltitude.value = photoPlanner.calcPhotoParamAltitudeOnGsd().toFixed()
            isCalculateAltitude = false
        }
    }

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
                    text: qsTr("Calculate Photo Plan")
                    horizontalAlignment: Text.AlignHCenter
                }

                RowLayout {
                    spacing: 5
                    Layout.margins: 10

                    Label {
                        Layout.fillWidth:   true
                        horizontalAlignment: Text.AlignLeft
                        text: qsTr("Uav Model")
                    }
                    Label {
                        Layout.fillWidth:   true
                        horizontalAlignment: Text.AlignLeft
                        text: paramUavModelName.currentText
                    }
                }

                RowLayout {
                    spacing: 5
                    Layout.margins: 10

                    Label {
                        Layout.fillWidth:   true
                        horizontalAlignment: Text.AlignLeft
                        text: qsTr("Camera Model")
                    }
                    Label {
                        Layout.fillWidth:   true
                        horizontalAlignment: Text.AlignLeft
                        text: paramCameraModelName.currentText
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
                    id: paramCalcAltitude
                    name: qsTr("Altitude, m")
                    from: 5
                    to: 5000
                    value: photoPlanner.altitude
                    onValueChanged: {
                        photoPlanner.altitude = value
                        photoPlannerParamsView.calculateGsd()
                    }
                }

                PhotoPlannerParamForm {
                    id: paramCalcGsd
                    name: qsTr("GSD, sm/pix")
                    from: 1
                    to: 300
                    onValueChanged: {
                        photoPlanner.gsd = value / 100
                        calculateAltitude()
                    }
                }

                PhotoPlannerParamForm {
                    visible: map.missionType == "Area"
                    name: qsTr("Azimuth, \xB0")
                    from: 0
                    to: 359
                    value: photoPlanner.azimuth
                    onValueChanged: { photoPlanner.azimuth = value }
                }

                PhotoPlannerParamForm {
                    visible: map.missionType == "Linear"
                    name: qsTr("Width, m")
                    from: 1
                    to: 1000
                    value: photoPlanner.width
                    onValueChanged: { photoPlanner.width = value }
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

            ColumnLayout {
                id: camerasView
                visible: SwipeView.isCurrentItem
                Layout.fillWidth: true

                property int modelIndex: -1
                property int prevModelIndex: -1
                Component.onCompleted: {
                    modelIndex = 0
                }
                onModelIndexChanged: {
                    paramCameraFocus.updatedModelIndex(modelIndex)
                    paramCameraLxMM.updatedModelIndex(modelIndex)
                    paramCameraLyMM.updatedModelIndex(modelIndex)
                    paramCameraAx.updatedModelIndex(modelIndex)
                    paramCameraAy.updatedModelIndex(modelIndex)
                    prevModelIndex = modelIndex
                    photoPlannerParamsView.calculateGsd()
                }

                ListModel {
                    id: camerasModel

                    ListElement {
                        name: "Sony A6000 [Sel20F28]"
                        focusMM: 20
                        lxMM: 15
                        lyMM: 22.5
                        ax: 3648
                        ay: 5472
                    }
                    ListElement {
                        name: "Sony A6000 [Sel35F18]"
                        focusMM: 35
                        lxMM: 15
                        lyMM: 22.5
                        ax: 3648
                        ay: 5472
                    }
                    ListElement {
                        name: "Sony S600 35"
                        focusMM: 35
                        lxMM: 15
                        lyMM: 22.5
                        ax: 2112
                        ay: 2816
                    }
                    ListElement {
                        name: "Sony S600 50"
                        focusMM: 50
                        lxMM: 15
                        lyMM: 22.5
                        ax: 2112
                        ay: 2816
                    }
                }

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
                   id: paramCameraModelName
                   Layout.fillWidth:   true
                   model: camerasModel
                   textRole: "name"
                   onCurrentIndexChanged: {
                       camerasView.modelIndex = currentIndex
                   }
                   onCurrentTextChanged: {
                       photoPlanner.cameraModel = currentText;
                   }
               }

               PhotoPlannerParamForm {
                   id: paramCameraFocus
                   name: qsTr("Focus Range, mm")
                   from: 1
                   to: 100

                   signal updatedModelIndex(int newIndex)
                   onUpdatedModelIndex: {
                       value = camerasModel.get(newIndex).focusMM
                   }
                   onValueChanged: {
                       photoPlanner.focusRange = value
                       if (camerasView.prevModelIndex == camerasView.modelIndex)
                           camerasModel.setProperty(camerasView.modelIndex,"focusMM", value)
                   }
               }
               PhotoPlannerParamForm {
                   id: paramCameraLxMM
                   name: qsTr("Sensor lx, mm")
                   from: 1
                   to: 100

                   signal updatedModelIndex(int newIndex)
                   onUpdatedModelIndex: {
                       value = camerasModel.get(newIndex).lxMM
                   }
                   onValueChanged: {
                       photoPlanner.cameraLx = value
                       if (camerasView.prevModelIndex == camerasView.modelIndex)
                           camerasModel.setProperty(camerasView.modelIndex,"lxMM", value)
                   }
               }
               PhotoPlannerParamForm {
                   id: paramCameraLyMM
                   name: qsTr("Sensor ly, mm")
                   from: 1
                   to: 100

                   signal updatedModelIndex(int newIndex)
                   onUpdatedModelIndex: {
                       value = camerasModel.get(newIndex).lyMM
                   }
                   onValueChanged: {
                       photoPlanner.cameraLy = value
                       if (camerasView.prevModelIndex == camerasView.modelIndex)
                           camerasModel.setProperty(camerasView.modelIndex,"lyMM", value)
                   }
               }
               PhotoPlannerParamForm {
                   id: paramCameraAx
                   name: qsTr("Sensor ax, pix")
                   from: 500
                   to: 10000

                   signal updatedModelIndex(int newIndex)
                   onUpdatedModelIndex: {
                       value = camerasModel.get(newIndex).ax
                   }
                   onValueChanged: {
                       photoPlanner.cameraAx = value
                       if (camerasView.prevModelIndex == camerasView.modelIndex)
                           camerasModel.setProperty(camerasView.modelIndex,"ax", value)
                   }
               }
               PhotoPlannerParamForm {
                   id: paramCameraAy
                   name: qsTr("Sensor ay, pix")
                   from: 500
                   to: 10000

                   signal updatedModelIndex(int newIndex)
                   onUpdatedModelIndex: {
                       value = camerasModel.get(newIndex).ay
                   }
                   onValueChanged: {
                       photoPlanner.cameraAy = value
                       if (camerasView.prevModelIndex == camerasView.modelIndex)
                           camerasModel.setProperty(camerasView.modelIndex,"ay", value)
                   }
               }
            }

            ColumnLayout {
                id: uavsView
                visible: SwipeView.isCurrentItem
                Layout.fillWidth: true

                property int modelIndex: -1
                property int prevModelIndex: -1
                Component.onCompleted: {
                    modelIndex = 0
                }
                onModelIndexChanged: {
                    paramUavTime.updatedModelIndex(modelIndex)
                    paramUavComm.updatedModelIndex(modelIndex)
                    paramUavSpeed.updatedModelIndex(modelIndex)
                    paramUavRoll.updatedModelIndex(modelIndex)
                    prevModelIndex = modelIndex
                }

                ListModel {
                    id: uavsModel

                    ListElement {
                        name: "Plane 1"
                        flightTimeMinutes: 60
                        flightSpeedKmPerH: 40
                        maxRollDeg: 30
                        commRadiusKm: 25
                    }
                    ListElement {
                        name: "Plane 2"
                        flightTimeMinutes: 120
                        flightSpeedKmPerH: 60
                        maxRollDeg: 30
                        commRadiusKm: 50
                    }
                    ListElement {
                        name: "Quadro 1"
                        flightTimeMinutes: 40
                        flightSpeedKmPerH: 30
                        maxRollDeg: 90
                        commRadiusKm: 5
                    }
                    ListElement {
                        name: "Quadro 2"
                        flightTimeMinutes: 60
                        flightSpeedKmPerH: 60
                        maxRollDeg: 90
                        commRadiusKm: 15
                    }
                }

                Label {
                    Layout.fillWidth:   true
                    Layout.columnSpan:  2
                    text: qsTr("UAV Parameters")
                    horizontalAlignment: Text.AlignHCenter
                }

                Label {
                    Layout.fillWidth:   true
                    text: qsTr("UAV Model")
                }
                ComboBox {
                    id: paramUavModelName
                    Layout.fillWidth:   true
                    model: uavsModel
                    textRole: "name"
                    onCurrentIndexChanged: {
                        uavsView.modelIndex = currentIndex
                    }
                }

                PhotoPlannerParamForm {
                    id: paramUavTime
                    name: qsTr("Flight Time, min")
                    from: 1
                    to: 240

                    signal updatedModelIndex(int newIndex)
                    onUpdatedModelIndex: {
                        value = uavsModel.get(newIndex).flightTimeMinutes
                    }
                    onValueChanged: {
                        if (uavsView.prevModelIndex == uavsView.modelIndex)
                            uavsModel.setProperty(uavsView.modelIndex,"flightTimeMinutes", value)
                    }
                }

                PhotoPlannerParamForm {
                    id: paramUavSpeed
                    name: qsTr("Flight Speed, kmph")
                    from: 1
                    to: 200

                    signal updatedModelIndex(int newIndex)
                    onUpdatedModelIndex: {
                        value = uavsModel.get(newIndex).flightSpeedKmPerH
                    }
                    onValueChanged: {
                        photoPlanner.speed = value;
                        if (uavsView.prevModelIndex == uavsView.modelIndex)
                            uavsModel.setProperty(uavsView.modelIndex,"flightSpeedKmPerH", value)
                    }
                }

                PhotoPlannerParamForm {
                    id: paramUavComm
                    name: qsTr("Comm Radius, km")
                    from: 1
                    to: 120

                    signal updatedModelIndex(int newIndex)
                    onUpdatedModelIndex: {
                        value = uavsModel.get(newIndex).commRadiusKm
                    }
                    onValueChanged: {
                        if (uavsView.prevModelIndex == uavsView.modelIndex)
                            uavsModel.setProperty(uavsView.modelIndex,"commRadiusKm", value)
                    }
                }

                PhotoPlannerParamForm {
                    id: paramUavRoll
                    name: qsTr("Max Roll, \xB0")
                    from: 1
                    to: 90

                    signal updatedModelIndex(int newIndex)
                    onUpdatedModelIndex: {
                        value = uavsModel.get(newIndex).maxRollDeg
                    }
                    onValueChanged: {
                        photoPlanner.maxRoll = value
                        if (uavsView.prevModelIndex == uavsView.modelIndex)
                            uavsModel.setProperty(uavsView.modelIndex,"maxRollDeg", value)
                    }
                }

            }
        }

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
