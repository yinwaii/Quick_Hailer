#include "mapmanager.h"

//MapManager *MapManager::getMapManager(QQuickWidget *widget)
//{
//    MapManager *manager = widget->rootObject()->findChild<MapManager *>("mapManager");
//    manager->setRoot(widget);
//    return manager;
//}

MapManager *MapManager::getManager(QQuickWidget *root, QString id)
{
    return root->rootObject()->findChild<MapManager *>(id);
}

MapManager::MapManager(QObject *parent) : QObject(parent), rm(), m_selectStatus(0)
{
    //    assert(widget != nullptr);
    connect(&rm, &RouteManager::updateLine, this, &MapManager::selectedPlanning);
}

QVariantList MapManager::gridList() const
{
    return m_gridList;
}

QVariantList MapManager::coordinateList() const
{
    return m_coordinateList;
}

QGeoCoordinate MapManager::coordinateFrom() const
{
    return m_coordinateFromTo.first;
}

QGeoCoordinate MapManager::coordinateTo() const
{
    return m_coordinateFromTo.second;
}

int MapManager::selectStatus() const
{
    return m_selectStatus;
}

void MapManager::setCoordinateList(const QVariantList &coordinateList)
{
    if (m_coordinateList != coordinateList) {
        m_coordinateList = coordinateList;
        emit updateCoordinateList();
    }
}

void MapManager::setCoordinateFrom(const QGeoCoordinate &coordinateFrom)
{
    if (m_coordinateFromTo.first != coordinateFrom) {
        m_coordinateFromTo.first = coordinateFrom;
        emit updateCoordinateFrom();
    }
}

void MapManager::setCoordinateTo(const QGeoCoordinate &coordinateTo)
{
    if (m_coordinateFromTo.second != coordinateTo) {
        m_coordinateFromTo.second = coordinateTo;
        emit updateCoordinateTo();
    }
}

void MapManager::setSelectStatus(const int &status)
{
    if (m_selectStatus != status) {
        m_selectStatus = status;
        emit updateSelectStatus();
    }
}

void MapManager::initGrids()
{
    m_gridList = DataBase::dataBase.getGrid();
    emit updateGrid();
    emit updateGridList();
}

void MapManager::updateHeatEntry(double start, double end, int step)
{
    m_gridList = DataBase::dataBase.getEntryExit(start, end, step);
    emit updateHeatEntry();
    emit updateGridList();
}

void MapManager::updateHeatExit(double start, double end, int step)
{
    m_gridList = DataBase::dataBase.getEntryExit(start, end, step);
    emit updateHeatExit();
    emit updateGridList();
}

void MapManager::updateRoute(double time)
{
    m_coordinateList = DataBase::dataBase.getRoute(time);
    emit updateCoordinateList();
}

void MapManager::selectFrom()
{
    m_selectStatus = 1;
    qDebug() << "From";
    emit updateSelectStatus();
}

void MapManager::selectTo()
{
    m_selectStatus = 2;
    qDebug() << "To";
    emit updateSelectStatus();
}

void MapManager::selectPlanning()
{
    if (m_coordinateFromTo.first.isValid() && m_coordinateFromTo.second.isValid()) {
        rm.setRoute(m_coordinateFromTo.first, m_coordinateFromTo.second);
    }
}

void MapManager::selectedPlanning()
{
    m_coordinateList.clear();
    QVariantMap tmp;
    foreach (QGeoCoordinate coord, rm.getPath())
        m_coordinateList.push_back(QVariant::fromValue(coord));
    tmp["road"] = m_coordinateList;
    m_coordinateList.clear();
    m_coordinateList.push_back(tmp);
    foreach (QGeoManeuver maneuver, rm.getManeuver()) {
        qDebug() << maneuver.instructionText() << maneuver.distanceToNextInstruction();
    }
    emit updateRoutePlanning();
}
//void MapManager::updateRoute(double time)
//{
//    m_coordinateList.clear();
//    QVariantList routes = DataBase::dataBase.getRoute(time);
//    routeCount = routes.length();
//    foreach (QVariant route, routes) {
//        rm.setRoute(route.toMap()["origin"].value<QGeoCoordinate>(),
//                    route.toMap()["destination"].value<QGeoCoordinate>());
//    }
//}

//void MapManager::updatedRoute()
//{
//    m_coordinateList.push_back(QVariant::fromValue(rm.getPath()));
//    qDebug() << "updated" << routeCount;
//    routeCount--;
//    if (routeCount == 0)
//        emit updateCoordinateList();
//}
