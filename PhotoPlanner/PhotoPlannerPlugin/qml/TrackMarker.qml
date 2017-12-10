import QtQuick 2.9;
import QtLocation 5.9;

MapQuickItem {
    id: marker
    anchorPoint.x: image.width/2
    anchorPoint.y: image.height
    sourceItem: Image {
        id: image
        fillMode: Image.PreserveAspectFit
        sourceSize.height: 24
        height: sourceSize.height
        source: "icons/place_sel.svg"

        Text{
            id: markerNumber
            y: image.height
            color: "darkblue"
            font.bold: true
            font.pixelSize: 14
            width:27
            horizontalAlignment: Text.AlignHCenter
        }
    }
    function setNumber(number) {
        markerNumber.text = number
    }

    function setPosition(toCoordinate) {
        coordinate = toCoordinate
    }
}



//MapCircle {
//    id: trackMarker

//    border.width: 2
//    border.color: "darkblue"
//    color: "blue"
//    radius: 10
//    opacity: 0.25

//    function setNumber(number) {
////        trackText.text = number
//    }

//    function setPosition(coordinate) {
//        center = coordinate
//    }

//    MouseArea {
//        anchors.fill: parent
//    }
////    sourceItem: Image {
////        fillMode: Image.PreserveAspectFit
////        anchors.centerIn: parent
////        sourceSize.height: 20
////        height: sourceSize.height
////        source: "icons/place.svg"

////        Text {
////            id: trackText
////            x:20
////            y:20
////            //anchors.centerIn: parent
////        }

////        MouseArea {
////            anchors.fill: parent
////        }
////    }

//}
