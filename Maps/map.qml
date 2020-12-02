import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6
Rectangle {
    width: 512
    height: 512
    visible: true

    Plugin {
        id: mapPlugin
        name: "osm" // "mapboxgl", "esri", ...
        // specify plugin parameters if necessary
        // PluginParameter {
        //     name:
        //     value:
        // }
    }

    Map {
        anchors.fill: parent
        plugin: mapPlugin
//        center: Position.coordinate
        center: QtPositioning.coordinate(30.67, 104.05) // Oslo
        zoomLevel: 14
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/
