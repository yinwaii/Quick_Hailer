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
                opacity: 0.2
                topLeft: QtPositioning.coordinate(grid.x,grid.y)
                bottomRight: QtPositioning.coordinate(grid.x+grid.width,grid.y+grid.height)
                MouseArea {
                    anchors.fill: mapGridRectangle
                    onPressed: {
                        console.log("clicked",index)
                        mapGridRectangle.color="red"
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
                line.width: 2
                line.color: "black"
//                opacity: count
//                path: [origin,destination]
                path:(index<mapManager.coordinateList.length)?mapManager.coordinateList[index].paths:[]
                Component.onCompleted: {
//                    console.log(origin,destination)
                    console.log(index)

                }
            }
//            delegate:MapRoute {
//                id:mapConnectionRoute
//                route: routes
//                line.color: "blue"
//                line.width: 1
//                smooth: true
//                opacity: 0.8
//                Component.onCompleted: {
//                }
//            }
        }
//        MapItemView {
//                        model: RouteModel {
//                            id: mapConnectionRouteModel
//                            plugin: mapPlugin
//                            autoUpdate: false
//                            query: mapConnectionRouteQuery
//                        }
//                        RouteQuery {
//                            id:mapConnectionRouteQuery
//                            travelModes: RouteQuery.MostEconomicRoute
//                            Component.onCompleted: {
//                                console.log(waypoints)
//                                mapConnectionRouteModel.update()
//                            }
//                        }

//                        delegate: MapRoute {
//                            route: routeData
//                            line.color: "blue"
//                            line.width: 1
//                            smooth: true
//                            opacity: 0.8
//                            Component.onCompleted: {
//                                mapConnectionModel.append({"routes":routeData})
//                            }
//                        }
//                    }
        MapItemView{
            id: mapRouteView
            model: ListModel {
                id: mapRouteModel
            }

            delegate: MapItemGroup {
                MapPolyline {
                id: mapRouteLine
                line.width: 1
                line.color: "blue"
                path: [origin,destination]
                Component.onCompleted: {
//                    console.log(origin,destination)
                }
            }
//                MapQuickItem {
//                        id:mapRouteMarker
//                        sourceItem: Image{
//                            id: mapRouteImage
//                            source: "marker.png"

//                        }
//                        coordinate: QtPositioning.coordinate(origin.latitude,origin.longitude)
//                        anchorPoint.x: image.width / 2
//                        anchorPoint.y: image.height
//                    }
            }
        }

        MapItemView{
            id: mapDotsView
            model: ListModel {
                id: mapDotsModel
            }

            delegate: MapItemGroup {
                MapQuickItem {
                        id:mapDotsOriginMarker
                        sourceItem: Image{
                            id: mapDotsOriginImage
                            source: "marker.png"

                        }
                        coordinate: QtPositioning.coordinate(origin.latitude,origin.longitude)
                        anchorPoint.x: image.width / 2
                        anchorPoint.y: image.height
                    }
                MapQuickItem {
                        id:mapDotsDestinationMarker
                        sourceItem: Image{
                            id: mapDotsDestinationImage
                            source: "marker.png"

                        }
                        coordinate: QtPositioning.coordinate(destination.latitude,destination.longitude)
                        anchorPoint.x: image.width / 2
                        anchorPoint.y: image.height
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
                mapGridModel.clear()
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
                mapRouteModel.clear()
                for(var i=0;i<coordinateList.length;i++)
                {
//                    console.log(coordinateList[i],coordinateList[i].paths)
                    mapConnectionModel.append(coordinateList[i])
//                                        mapConnectionRouteQuery.clearWaypoints()
//                                        mapConnectionRouteQuery.addWaypoint(coordinateList[i].origin)
//                                        mapConnectionRouteQuery.addWaypoint(coordinateList[i].destination)
//                                        mapConnectionRouteModel.update()

                }
//                map.fitViewportToVisibleMapItems()
            }

            onUpdateSelectStatus: {
                console.log(selectStatus)
                mouseSelect.visible = true
            }

            onUpdateRoutePlanning: {
                mapRouteModel.clear()
                for(var i=0;i<coordinateList.length;i++)
                {
                    mapRouteModel.append(coordinateList[i])
//                    mapConnectionRouteQuery.clearWaypoints()
//                    mapConnectionRouteQuery.addWaypoint(coordinateList[i].origin)
//                    mapConnectionRouteQuery.addWaypoint(coordinateList[i].destination)
//                    mapConnectionRouteModel.update()
//                    console.log(coordinateList[i])
                }

//                mapRouteModel.insert(coordinateList[coordinateList.length-1])
//                console.log(coordinateList)
            }

            onUpdateDots: {
                mapDotsModel.clear()
                for(var i=0;i<coordinateList.length;i++)
                {
                    mapDotsModel.append(coordinateList[i])
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
