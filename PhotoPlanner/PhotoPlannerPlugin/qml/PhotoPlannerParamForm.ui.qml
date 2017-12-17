import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

GridLayout {
    id: thisParam
    property string name: "Param label"
    property real value: 0
    property real from: 0
    property real to: 100
    property real stepSize: 1
    property bool visibleSlider: true
    rowSpacing: 1
    columnSpacing: 5
    Layout.topMargin: 3
    Layout.bottomMargin: 3

    signal valueChangedByUser()

    rows: 2
    columns: 2

    Label {
        Layout.fillWidth: true
        text: parent.name
        textFormat: Text.PlainText
        wrapMode: Text.WordWrap
    }

    TextField {
        id: textField
        Layout.fillWidth: true
        text: parent.value
        horizontalAlignment: Text.AlignRight
    }

    Slider {
        id: slider
        padding: 0
        visible: parent.visibleSlider
        Layout.fillWidth: true
        Layout.columnSpan: 2
        from: parent.from
        to: parent.to
        value: parent.value
        stepSize: parent.stepSize
    }

    Connections {
        target: slider
        onMoved: {
            var val = slider.value.toFixed(0)
            thisParam.value = val
            valueChangedByUser()
        }
    }

    Connections {
        target: textField
        onEditingFinished: {
            var val = textField.text
            val = Math.min(val, thisParam.to)
            val = Math.max(val, thisParam.from)
            if (thisParam.value != val)
                thisParam.value = val
            valueChangedByUser()
        }
    }
}
