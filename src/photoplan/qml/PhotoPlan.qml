import QtQuick 2.4
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import com.uavos.map 1.0
import PhotoPlan 1.0

Item {
//    anchors.fill: parent
    property int leftBarOffset: 30
    signal zooming()
    onZooming: canvas.requestPaint();
    Button
    {
        y: 50
        height: 50
        width: 50
        text: "Calc"
        onClicked:  {
            pointView.model.recalc();
            canvas.requestPaint();
        }
    }

    Rectangle {
        height: parent.height
        width: 500
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        border.color: "lightgreen"
        border.width: 1
        color: "transparent"

        ColumnLayout {
            anchors.fill: parent
            spacing: 1

            Label {
                id: zoomLevel
                Layout.fillWidth: true
                font.pixelSize: 25
                font.bold: true
                text: "Zoom level: " + mapProvider.level
            }
            Label {
                id: mousePos
                Layout.fillWidth: true
                font.pixelSize: 25
                font.bold: true
                text: "MousePos: " + Math.round(mapProvider.mouseLL.x*10000)/10000 +
                      " " + Math.round(mapProvider.mouseLL.y*10000)/10000
            }
/*            Label {
                id: mouseState
                Layout.fillWidth: true
//                text: "MouseState: " + mouseArea.pressed ? "pressed" : "released"
//                var data = {'number': 25}; //coordList.count()
//                pointView.model.append(data);
                //pointView.model.append(data);
                //pointView.model.sync();
            }
*/
            ListView {
                id: pointView
                Layout.fillHeight: true
                Layout.fillWidth: true

               // model:     ListModel {
//                 model:     PhotoPlanModel {
                model:     PhotoPlanModel {
                     id: coordList
                }

                delegate: Text {
                   // font: zoomLevel.font
                    color: "white"
                    font.pixelSize: 25
                    font.bold: true
                    text: " Lon:" + Math.round(longitude*10000)/10000 +
                          " Lat:" + Math.round(latitude*10000)/10000 +
                          " Alt:" +altitude;
                }
            }


        //    Item {
        //        Layout.fillHeight: true
        //    }
        }
    }

    Canvas {
        id: canvas
        anchors.fill: parent
        canvasSize.width: mapProvider.levelTiles*mapProvider.tileSize//flickable.contentWidth
        canvasSize.height: mapProvider.levelTiles*mapProvider.tileSize//flickable.contentHeight
        tileSize.width: mapProvider.tileSize
        tileSize.height: mapProvider.tileSize
        canvasWindow.width: map.width
        canvasWindow.height: map.height
        renderStrategy: Canvas.Immediate//Threaded//Immediate //Threaded //.Cooperative
        renderTarget: Canvas.Image//Image //.FramebufferObject
        contextType: "2d"
        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();
            ctx.strokeStyle = "red";
            ctx.fillStyle = Qt.rgba(1.0, 0.3, 0.3, 0.1);
            ctx.beginPath();

            var mapPointX;
            var mapPointY;

            for (var i =0; i<pointView.model.rowCount(); i++)
            {
                mapPointX = map.sceneToWindowX(map.mapToSceneX(mapProvider.lonToX(pointView.model.get(i)['longitude'])))*map.ws;
                mapPointY = map.sceneToWindowY(map.mapToSceneY(mapProvider.latToY(pointView.model.get(i)['latitude'])))*map.hs;

                if(i>0)
                    ctx.lineTo(mapPointX, mapPointY);
                else
                    ctx.moveTo(mapPointX, mapPointY);
                ctx.stroke();
            }
            ctx.fill();
            ctx.closePath();

            for (var i =0; i<pointView.model.rowCount(); i++)
            {
                ctx.beginPath();
                mapPointX = map.sceneToWindowX(map.mapToSceneX(mapProvider.lonToX(pointView.model.get(i)['longitude'])))*map.ws;
                mapPointY = map.sceneToWindowY(map.mapToSceneY(mapProvider.latToY(pointView.model.get(i)['latitude'])))*map.hs;
                ctx.fillStyle = "red";
                ctx.arc(mapPointX, mapPointY, 7, 0, Math.PI * 2, false);
                ctx.fill();
                ctx.closePath();
                ctx.fillStyle = "white";
                ctx.font = "10px sans-serif"
                ctx.textAlign = "center"
                ctx.textBaseline = "middle"
                ctx.fillText(i, mapPointX, mapPointY);
            }

            console.log("Center at: "+mapPointY + ":" + mapPointX + ":"
                        + canvasWindow.width + ":" + canvasWindow.height);
        }
    }

    MouseArea {
        id: mouseArea
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }
        width : map.ws - leftBarOffset
        onClicked: {
/*            console.log("Clicked at-1: " + mapProvider.mouseLL.y + ":" +  mapProvider.mouseLL.x);
            console.log("Clicked at: "+mapProvider.latToY(mapProvider.mouseLL.y) + ":" + mapProvider.lonToX(mapProvider.mouseLL.x));
            console.log("Clicked at1: "+map.mapToSceneY(mapProvider.mouseLL.y) + ":" + map.mapToSceneX(mapProvider.mouseLL.x));
            console.log("Clicked at2: "+map.contentX + ":" +map.contentY);
            console.log("Clicked at3: "+mapProvider.shift.x+ ":" +mapProvider.shift.y);
*/
            //'number': pointView.model.count+1,
            var data = {'latitude': mapProvider.mouseLL.x, 'longitude': mapProvider.mouseLL.y}; //coordList.count()
            pointView.model.append1(mapProvider.mouseLL.x, mapProvider.mouseLL.y);
            canvas.requestPaint();
   //       pointView.model.sync();
        }
    }
    Component.onCompleted: {
        map.zoomingChanged.connect(zooming);
    }
}
