import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2

Rectangle {
    id: photoPlannerParamsView


    signal updatePhotoPlanAltitudeAndGsd()
    signal calculateGsd()
    signal calculateAltitude()

    signal updatePhotoPlanWidthAndRuns()
    signal calculateWidth()
    signal calculateRuns()

    signal updateUavRAndRoll()
    signal calculateUavR()
    signal calculateUavRoll()

    Connections {
        target: photoPlanner
        onCameraModelNameChanged: {
            var newIndex = paramCameraModelName.find(photoPlanner.cameraModelName)
            if (newIndex !== -1)
                paramCameraModelName.currentIndex = newIndex
        }
    }

    Connections {
        target: photoPlanner
        onUavModelNameChanged: {
            var newIndex = paramUavModelName.find(photoPlanner.uavModelName)
            if (newIndex !== -1)
                paramUavModelName.currentIndex = newIndex
        }
    }

    property bool isInitialized: false
    Component.onCompleted: {
        isInitialized = true
        updatePhotoPlanAltitudeAndGsd()
        updatePhotoPlanWidthAndRuns()
        updateUavRAndRoll()
    }

    property bool isPhotoGsdFixed: false
    onIsPhotoGsdFixedChanged: {
        isPhotoAltitudeFixed = !isPhotoGsdFixed
    }
    property bool isPhotoAltitudeFixed: true
    onIsPhotoAltitudeFixedChanged: {
        isPhotoGsdFixed = !isPhotoAltitudeFixed;
    }
    onUpdatePhotoPlanAltitudeAndGsd: {
        if (isPhotoAltitudeFixed) {
            calculateGsd()
        } else {
            calculateAltitude()
        }
    }

    property bool isCalculatingGsd: false
    onCalculateGsd: {
        if (isInitialized && !isCalculatingAltitude) {
            isCalculatingGsd = true
            paramCalcGsd.value = (photoPlanner.calcPhotoParamGsdOnAltitude() * 100).toFixed(2)
            isCalculatingGsd = false
            updatePhotoPlanWidthAndRuns()
        }
    }
    property bool isCalculatingAltitude: false
    onCalculateAltitude: {
        if (isInitialized && !isCalculatingGsd) {
            isCalculatingAltitude = true
            paramCalcAltitude.value = photoPlanner.calcPhotoParamAltitudeOnGsd().toFixed()
            isCalculatingAltitude = false
            updatePhotoPlanWidthAndRuns()
        }
    }


    property bool isUavRFixed: false
    onIsUavRFixedChanged: {
        isUavRollFixed = !isUavRFixed
    }
    property bool isUavRollFixed: true
    onIsUavRollFixedChanged: {
        isUavRFixed = !isUavRollFixed
    }
    onUpdateUavRAndRoll: {
        if (isUavRFixed) {
            calculateUavRoll()
        } else {
            calculateUavR()
        }
    }

    property bool isCalculatingUavR: false
    onCalculateUavR: {
        if (isInitialized && !isCalculatingUavRoll) {
            isCalculatingUavR = true
            paramUavR.value = photoPlanner.calcUavManeuverROnMaxRoll().toFixed(2)
            isCalculatingUavR = false
        }
    }
    property bool isCalculatingUavRoll: false
    onCalculateUavRoll: {
        if (isInitialized && !isCalculatingUavR) {
            isCalculatingUavRoll = true
            paramUavRoll.value = photoPlanner.calcUavMaxRollOnManeuverR().toFixed(2)
            isCalculatingUavRoll = false
        }
    }

    property bool isLinearPhotoWidthFixed: true
    onIsLinearPhotoWidthFixedChanged: {
        isLinearPhotoRunsFixed = !isLinearPhotoWidthFixed
    }
    property bool isLinearPhotoRunsFixed: false
    onIsLinearPhotoRunsFixedChanged: {
        isLinearPhotoWidthFixed = !isLinearPhotoRunsFixed;
    }

    onUpdatePhotoPlanWidthAndRuns: {
        if (map.missionType == "Linear") {
            if (isLinearPhotoWidthFixed) {
                calculateRuns()
            }
            else {
                calculateWidth()
            }
        }
    }

    property bool isCalculatingWidth: false
    onCalculateWidth: {
        if (isInitialized && !isCalculatingRuns) {
            isCalculatingWidth = true
            paramCalcWidth.value = photoPlanner.calcPhotoPlanWidth()
            isCalculatingWidth = false
        }
    }

    property bool isCalculatingRuns: false
    onCalculateRuns: {
        if (isInitialized && !isCalculatingWidth) {
            isCalculatingRuns = true;
            paramCalcRuns.value = photoPlanner.calcPhotoPlanRuns()
            isCalculatingRuns = false;
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5

        SwipeView {
            id: swipeView
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.fillWidth: true
            Layout.fillHeight: true

            currentIndex: tabBar.currentIndex

            Item {
                visible: SwipeView.isCurrentItem
                ColumnLayout {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Label {
                        Layout.fillWidth:   true
                        text: qsTr("Calculate Photo Plan")
                        horizontalAlignment: Text.AlignHCenter
                    }

                    PhotoPlannerParamSeparator {
                    }

                    RowLayout {
                        spacing: 5

                        Label {
                            Layout.fillWidth:   true
                            text: qsTr("UAV Model")
                        }
                        ComboBox {
                            id: paramUavModelName
                            Layout.minimumWidth: swipeView.width * 0.6
                            model: uavsModel
                            textRole: "name"
                            onCurrentIndexChanged: {
                                uavsView.modelIndex = currentIndex
                            }
                            onCurrentTextChanged: {
                                photoPlanner.uavModelName = currentText;
                            }
                        }
                    }

                    RowLayout {
                        spacing: 5

                        Label {
                            Layout.fillWidth:   true
                            text: qsTr("Camera Model")
                        }
                        ComboBox {
                            id: paramCameraModelName
                            Layout.minimumWidth: swipeView.width * 0.6
                            model: camerasModel
                            textRole: "name"
                            onCurrentIndexChanged: {
                                camerasView.modelIndex = currentIndex
                            }
                            onCurrentTextChanged: {
                                photoPlanner.cameraModelName = currentText;
                            }
                        }
                    }

                    PhotoPlannerParamSeparator {
                    }

                    PhotoPlannerParamForm {
                        name: qsTr("Longitudinal overlap, %")
                        from: 0
                        to: 95
                        value: photoPlanner.longitOverlap
                        onValueChanged: {
                            photoPlanner.longitOverlap = value
                            updatePhotoPlanWidthAndRuns()
                        }
                    }

                    PhotoPlannerParamForm {
                        name: qsTr("Transverse overlap, %")
                        from: 0
                        to: 95
                        value: photoPlanner.transverseOverlap
                        onValueChanged: {
                            photoPlanner.transverseOverlap = value
                            updatePhotoPlanWidthAndRuns()
                        }
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
                        onValueChangedByUser: {
                            isPhotoAltitudeFixed = true
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
                        onValueChangedByUser: {
                            isPhotoGsdFixed = true
                        }
                    }

                    PhotoPlannerParamSeparator {
                        visible: map.missionType == "Area" || map.missionType == "Linear"
                    }

                    PhotoPlannerParamForm {
                        visible: map.missionType == "Area"
                        name: qsTr("Azimuth, \xB0")
                        from: 0
                        to: 359
                        value: photoPlanner.azimuth
                        onValueChanged: { photoPlanner.azimuth = value }
                    }

                    RowLayout {
                        visible: map.missionType == "Linear"

                        ColumnLayout {
                            PhotoPlannerParamForm {
                                id: paramCalcWidth
                                name: qsTr("Width, m")
                                from: 1
                                to: 1000
                                value: photoPlanner.linearWidth
                                onValueChanged: {
                                    photoPlanner.linearWidth = value
                                    calculateRuns()
                                }
                                onValueChangedByUser: {
                                    isLinearPhotoWidthFixed = true
                                }
                            }

                            PhotoPlannerParamForm {
                                id: paramCalcRuns
                                name: qsTr("Runs")
                                from: 1
                                to: 10
                                value: photoPlanner.linearRuns
                                onValueChanged: {
                                    photoPlanner.linearRuns = value
                                    calculateWidth()
                                }
                                onValueChangedByUser: {
                                    isLinearPhotoRunsFixed = true
                                }
                            }
                        }
                    }
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

            Item {
                visible: SwipeView.isCurrentItem
                ColumnLayout {
                    id: camerasView
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    property int modelIndex: -1
                    property int prevModelIndex: -1
                    Component.onCompleted: {
                        modelIndex = 0
                    }
                    onModelIndexChanged: {
                        paramCameraModelNameEdit.updatedModelIndex(modelIndex)
                        paramCameraFocus.updatedModelIndex(modelIndex)
                        paramCameraLxMM.updatedModelIndex(modelIndex)
                        paramCameraLyMM.updatedModelIndex(modelIndex)
                        paramCameraAx.updatedModelIndex(modelIndex)
                        paramCameraAy.updatedModelIndex(modelIndex)
                        prevModelIndex = modelIndex
                        photoPlannerParamsView.updatePhotoPlanAltitudeAndGsd()
                    }

                    Label {
                        Layout.fillWidth:   true
                        Layout.columnSpan:  2
                        text: qsTr("Cameras Parameters")
                        horizontalAlignment: Text.AlignHCenter
                    }

                    PhotoPlannerParamSeparator {  }

                    RowLayout {
                        spacing: 5

                        Label {
                            Layout.fillWidth:   true
                            text: qsTr("Camera Model")
                        }
                        TextField {
                            id: paramCameraModelNameEdit
                            Layout.fillWidth:   true
                            signal updatedModelIndex(int newIndex)
                            onUpdatedModelIndex: {
                                text = camerasModel.get(newIndex).name
                            }
                            onEditingFinished: {
                                camerasModel.get(camerasView.modelIndex).name=text
                            }
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
                            if (camerasView.prevModelIndex == camerasView.modelIndex) {
                                camerasModel.get(camerasView.modelIndex).focusMM = value
                            }
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
                                camerasModel.get(camerasView.modelIndex).lxMM = value
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
                                camerasModel.get(camerasView.modelIndex).lyMM = value
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
                                camerasModel.get(camerasView.modelIndex).ax = value
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
                                camerasModel.get(camerasView.modelIndex).ay = value
                        }
                    }
                }
            }

            Item {
                visible: SwipeView.isCurrentItem
                ColumnLayout {
                    id: uavsView
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    property int modelIndex: -1
                    property int prevModelIndex: -1
                    Component.onCompleted: {
                        modelIndex = 0
                    }
                    onModelIndexChanged: {
                        paramUavModelNameEdit.updatedModelIndex(modelIndex)
                        paramUavTime.updatedModelIndex(modelIndex)
                        paramUavComm.updatedModelIndex(modelIndex)
                        paramUavSpeed.updatedModelIndex(modelIndex)
                        paramUavRoll.updatedModelIndex(modelIndex)
                        paramUavR.updatedModelIndex(modelIndex)
                        prevModelIndex = modelIndex
                        updateUavRAndRoll()
                    }

                    Label {
                        Layout.fillWidth:   true
                        Layout.columnSpan:  2
                        text: qsTr("UAV Parameters")
                        horizontalAlignment: Text.AlignHCenter
                    }

                    PhotoPlannerParamSeparator {
                    }

                    RowLayout {
                        spacing: 5

                        Label {
                            Layout.fillWidth:   true
                            text: qsTr("UAV Model")
                        }
                        TextField {
                            id: paramUavModelNameEdit
                            Layout.fillWidth:   true
                            signal updatedModelIndex(int newIndex)
                            onUpdatedModelIndex: {
                                text = uavsModel.get(newIndex).name
                            }
                            onEditingFinished: {
                                uavsModel.get(uavsView.modelIndex).name=text
                            }
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
                                uavsModel.get(uavsView.modelIndex).flightTimeMinutes = value
                        }
                    }

                    PhotoPlannerParamForm {
                        id: paramUavSpeed
                        name: qsTr("Flight Speed, mps")
                        from: 1
                        to: 200

                        signal updatedModelIndex(int newIndex)
                        onUpdatedModelIndex: {
                            value = uavsModel.get(newIndex).flightSpeedMPerS
                        }
                        onValueChanged: {
                            photoPlanner.speed = value;
                            if (uavsView.prevModelIndex == uavsView.modelIndex)
                                uavsModel.get(uavsView.modelIndex).flightSpeedMPerS = value
                            updateUavRAndRoll()
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
                                uavsModel.get(uavsView.modelIndex).commRadiusKm = value
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
                                uavsModel.get(uavsView.modelIndex).maxRollDeg = value
                            calculateUavR()
                        }
                        onValueChangedByUser: {
                            isUavRollFixed = true;
                        }
                    }

                    PhotoPlannerParamForm {
                        id: paramUavR
                        name: qsTr("Maneuver R, m")
                        from: 0
                        to: 5000

                        signal updatedModelIndex(int newIndex)
                        onValueChanged: {
                            photoPlanner.uavManeuverR = value
                            calculateUavRoll()
                        }
                        onValueChangedByUser: {
                            isUavRFixed = true;
                        }
                    }
                }

            }
            Item {
                visible: SwipeView.isCurrentItem

                ColumnLayout {
                    id: optionsView

                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    ListModel {
                        id: mapTypesModel
                        Component.onCompleted: {
                            for (var i = 0; i<map.supportedMapTypes.length; i++){
                                mapTypesModel.append(
                                            {"description":map.supportedMapTypes[i].description})
                            }
                        }
                    }

                    Label {
                        Layout.fillWidth:   true
                        text: qsTr("Common options")
                        horizontalAlignment: Text.AlignHCenter
                    }

                    PhotoPlannerParamSeparator {
                    }

                    Label {
                        text: qsTr("Choose map view type")
                    }

                    ComboBox {
                        Layout.fillWidth:   true
                        model: mapTypesModel
                        textRole: "description"
                        Component.onCompleted: {
                            currentIndex = photoPlannerOptions.mapTypeIndex
                        }
                        onCurrentIndexChanged: {
                            map.activeMapType = map.supportedMapTypes[currentIndex]
                            photoPlannerOptions.mapTypeIndex = currentIndex
                        }
                    }

                    PhotoPlannerParamSeparator {
                    }

                    CheckBox {
                        Layout.fillWidth: true
                        checked: map.photoPrintsVisible
                        text: qsTr("Enable photo prints")
                        onCheckedChanged: {
                            if (checked != map.photoPrintsVisible)
                                map.photoPrintsVisible = checked
                        }

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
                text: qsTr("Planner")
            }
            TabButton {
                text: qsTr("Track")
            }
            TabButton {
                text: qsTr("Camera")
            }
            TabButton {
                text: qsTr("UAV")
            }
            TabButton {
                text: qsTr("Options")
            }
        }
    }
}
