import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2
import QtLocation 5.9
import QtPositioning 5.8

Item {


    Plugin {
        id:     mapPlugin
        name:   "nokia"
        PluginParameter { name: "here.app_id"; value: "0nq9YLyTnyFO4uiCkHQ4" }
        PluginParameter { name: "here.token"; value: "iRlA2YudpvvSpwzXcrjnPg" }
        PluginParameter { name: "mapping.cache.directory"; value: "/tmp/maps" }
    }

    Map {
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(-27.47, 153.025)
        zoomLevel: 14
    }
/*
    Rectangle {
        anchors.fill: parent
  //      width: 200
    //    height:200
        color: "blue"
    }
*/
    ToolBar {
        width:          48
        anchors.top:    parent.top
        anchors.left:   parent.left
        anchors.bottom: parent.bottom

        ColumnLayout {
            anchors.fill: parent
            ToolButton {
                text: "1"
            }
            ToolButton {
                text: "2"
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }

    Text {
        anchors.top: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10
        text: "Photo Planner"
    }

}
