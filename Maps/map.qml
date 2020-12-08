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
            id: mapEntryView
            model: ListModel {
                id: mapEntryModel
            }

            delegate: MapRectangle {
                id: mapEntryRectangle
                border.width: 0
                color: "red"
                opacity: (entry>0)?(entry):0.2
                topLeft: QtPositioning.coordinate(grid.x,grid.y)
                bottomRight: QtPositioning.coordinate(grid.x+grid.width,grid.y+grid.height)
                Component.onCompleted: {
//                    if(entry==undefined)
//                        opacity=0.2
                }
            }
        }
        MapItemView{
            id: mapExitView
            model: ListModel {
                id: mapExitModel
            }

            delegate: MapRectangle {
                id: mapExitRectangle
                border.width: 0
                color: "blue"
                opacity: (exit>0)?(exit):0.2
                topLeft: QtPositioning.coordinate(grid.x,grid.y)
                bottomRight: QtPositioning.coordinate(grid.x+grid.width,grid.y+grid.height)
                Component.onCompleted: {
//                    if(entry==undefined)
//                        opacity=0.2
                }
            }
        }

        MapItemView{
            id: mapGridsView
            model: ListModel {
                id: mapGridModel
            }

            delegate: MapRectangle {
                id: mapGridRectangle
                border.width: 1
                color: "red"
                opacity: 0
                topLeft: QtPositioning.coordinate(grid.x,grid.y)
                bottomRight: QtPositioning.coordinate(grid.x+grid.width,grid.y+grid.height)
                Component.onCompleted: {
//                    if(entry==undefined)
//                        opacity=0.2
                }
            }
        }

        MapManager {
            id:mapManager
            objectName: "mapManager"
            onUpdateGrid: {
                for(var i=0;i<gridList.length;i++)
                    mapGridModel.append(gridList[i])
            }

            onUpdateHeatEntry: {
                for(var i=0;i<gridList.length;i++)
                {
                    if(gridList[i]["entry"]>0)
                    {
                        mapEntryModel.append(gridList[i])
                    }
                }
            }

            onUpdateHeatExit: {
                for(var i=0;i<gridList.length;i++)
                {
                    if(gridList[i]["exit"]>0)
                    {
                        mapExitModel.append(gridList[i])
                    }
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
