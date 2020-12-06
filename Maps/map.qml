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

        MapItemView{
            id: mapGridsView
            model: ListModel {
                id: mapGridModel
            }

            delegate: MapRectangle {
                id: mapRectangle
                border.width: 1
                color: "red"
                opacity: (entry>0)?(2*entry):0.2
                topLeft: QtPositioning.coordinate(grid.x,grid.y)
                bottomRight: QtPositioning.coordinate(grid.x+grid.width,grid.y+grid.height)
                Component.onCompleted: {
//                    if(entry==undefined)
//                        opacity=0.2
                }
            }
            MapManager {
                id:mapManager
                objectName: "mapManager"
                onUpdateGrid: {
                    for(var i=0;i<gridList.length;i++)
                        mapGridModel.append(gridList[i])
                }

                onUpdateHeat: {
                    for(var i=0;i<gridList.length;i++)
                        mapGridModel.set(i,gridList[i])
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
