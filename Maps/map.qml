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
//                color: "red"
                opacity: 1
                topLeft: QtPositioning.coordinate(grid.x,grid.y)
                bottomRight: QtPositioning.coordinate(grid.x+grid.width,grid.y+grid.height)
                MouseArea {
                    anchors.fill: mapGridRectangle
                    onPressed: {
                        console.log("clicked")
                    }
                }
            }
        }
        MapItemView{
            id: mapConnectionView
            model: ListModel {
                id: mapConnectionModel
            }

            delegate: MapPolyline {
                id: mapConnectionLine
                line.width: 1
                line.color: "black"
                path: [origin,destination]
                Component.onCompleted: {
                    console.log(origin,destination)
                }
            }
        }
        MapItemView{
            id: mapRouteView
            model: ListModel {
                id: mapRouteModel
            }

            delegate: MapPolyline {
                id: mapRouteLine
                line.width: 1
                line.color: "blue"
                path: road
                Component.onCompleted: {
                    console.log(road)
                }
            }
        }
        MapQuickItem {
                id:marker
                visible: false
                sourceItem: Image{
                    id: image
                    source: "marker.png"

                }
                coordinate: map.center
                anchorPoint.x: image.width / 2
                anchorPoint.y: image.height
            }
            MouseArea {
                id: mouseSelect
                visible: false
                anchors.fill: parent
                onPressed: {
                    marker.visible=true
                    marker.coordinate = map.toCoordinate(Qt.point(mouse.x,mouse.y))
                    if(mapManager.selectStatus===1) {
                        mapManager.coordinateFrom=map.toCoordinate(Qt.point(mouse.x,mouse.y))
                        console.log("from",mapManager.coordinateFrom)
                    }
                    else if(mapManager.selectStatus===2) {
                        mapManager.coordinateTo=map.toCoordinate(Qt.point(mouse.x,mouse.y))
                        console.log("to",mapManager.coordinateTo)
                    }
                    visible = false
                }
            }
        MapManager {
            id:mapManager
            objectName: "mapManager"
            onUpdateGrid: {
                for(var i=0;i<gridList.length;i++)
                    mapGridModel.append(gridList[i])
                map.fitViewportToVisibleMapItems()
            }

            onUpdateHeatEntry: {
                for(var i=0;i<gridList.length;i++)
                {
                    if(gridList[i]["entry"]>0)
                    {
                        mapEntryModel.append(gridList[i])
                    }
                }
                map.fitViewportToVisibleMapItems()
            }

            onUpdateHeatExit: {
                for(var i=0;i<gridList.length;i++)
                {
                    if(gridList[i]["exit"]>0)
                    {
                        mapExitModel.append(gridList[i])
                    }
                }
                map.fitViewportToVisibleMapItems()
            }

            onUpdateCoordinateList: {
                for(var i=0;i<coordinateList.length;i++)
                {
                    mapConnectionModel.append(coordinateList[i])
                }
                map.fitViewportToVisibleMapItems()
            }

            onUpdateSelectStatus: {
                console.log(selectStatus)
                mouseSelect.visible = true
            }

            onUpdateRoutePlanning: {
                for(var i=0;i<coordinateList.length;i++)
                {
                    mapRouteModel.append(coordinateList[i])
                    console.log(coordinateList[i])
                }
//                console.log(coordinateList)
            }
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/
