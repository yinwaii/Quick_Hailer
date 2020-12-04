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
        zoomLevel: 11
        MapManager {
            id: mapManager
            property MapRectangle rectangle
            objectName: "mapManager"
//            onGridDataChanged: {
//                updateModel()
//            }

//            onGridsChanged: {
//                for(var i=0;i<grids.length;i++)
//                {
//                    rectangle=Qt.createQmlObject('import QtLocation 5.6; MapRectangle{}',map)
//                    rectangle.objectName="mapGrid"+i.toString()
//                    rectangle.border.width=1;
////                    rectangle.color="red"
//                    rectangle.opacity=0.2
//                    rectangle.topLeft=QtPositioning.coordinate(grids[i].x,grids[i].y)
//                    rectangle.bottomRight=QtPositioning.coordinate(grids[i].x+grids[i].width,grids[i].y+grids[i].height)
//                    map.addMapItem(rectangle)
//                    map.update()
//                    console.log(rectangle)
//                    console.log(grids[i].x,grids[i].y,grids[i].width,grids[i].height)

//                }
//                myGridModel=gridData;
//            }
//            onHeatChanged: {
//                console.log(map.mapItems)
//                for(var i=0;i<heat.length;i++)
//                {
//                    console.log(heat)
//                    console.log(heat[i].x,heat[i].y)
//                }
//            }
        }
        MapItemView{
            id: mapGrids
            model: myGridModel
            delegate: MapRectangle {
                id: mapRectangle
                border.width: 1
                opacity: 0.2
                topLeft: QtPositioning.coordinate(modelData["grid"].x,modelData["grid"].y)
                bottomRight: QtPositioning.coordinate(modelData["grid"].x+modelData["grid"].width,modelData["grid"].y+modelData["grid"].height)
            }
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/
