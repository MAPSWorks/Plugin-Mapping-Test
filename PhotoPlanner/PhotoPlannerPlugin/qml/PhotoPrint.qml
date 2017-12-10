import QtQuick 2.5
import QtLocation 5.6

MapPolygon {
    id: photoPrint

    border.color: "#8021be2b"
    color:  "#2021be2b"

    border.width: 1
    smooth: true
    //opacity: 0.5
    visible: false

    function setBorder(coordinate0, coordinate1, coordinate2, coordinate3 ) {
        var newPath = [coordinate0, coordinate1, coordinate2, coordinate3];
        path = newPath;
        visible = true
    }

    MouseArea {
        hoverEnabled: true
        anchors.fill: parent
        id: mousearea
        drag.target: parent

        onEntered: {
            photoPrint.border.color = "yellow"
            border.width = 2
        }
        onExited: {
            photoPrint.border.color = "#8021be2b"
            border.width = 1
        }
    }
}
