import QtQuick 2.0
import QtLocation 5.12
import QtPositioning 5.12
import MapManager 1.0
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
        id: map
        anchors.fill: parent
        plugin: mapPlugin
//        center: Position.coordinate
        center: QtPositioning.coordinate(30.67, 104.05) // Oslo
        zoomLevel: 10
        Component.onCompleted: {
            circle = Qt.createQmlObject('import QtLocation 5.6; MapCircle {}', page)
            circle.center = positionSource.position.coordinate
            circle.radius = 5000.0
            circle.color = 'green'
            circle.border.width = 3
            map.addMapItem(circle)
        }
        MapManager {
            id: mapManager
            property MapRectangle rectangle
            objectName: "mapManager"
            onGridsChanged: {
                for(var i=0;i<grids.length;i++)
                {
                    rectangle=Qt.createQmlObject('import QtLocation 5.6; MapRectangle{}',map)
                    rectangle.border.width=1;
//                    rectangle.color="red"
                    rectangle.opacity=0.2
                    rectangle.topLeft=QtPositioning.coordinate(grids[i].x,grids[i].y)
                    rectangle.bottomRight=QtPositioning.coordinate(grids[i].x+grids[i].width,grids[i].y+grids[i].height)
                    map.addMapItem(rectangle)
                    map.update()
    //                rectangle.left=
                    console.log(rectangle)
                    console.log(grids[i].x,grids[i].y,grids[i].width,grids[i].height)
    //                rectangle.topLeft=grids[i]
                }
            }
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/
