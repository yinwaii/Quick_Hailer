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
    connect(&rm, &RouteManager::updateFlow, this, &MapManager::updatedRoute);
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

//void MapManager::updateRoute(double time, int step)
//{
//    m_coordinateList = DataBase::dataBase.getRoute(time, step);
//    emit updateCoordinateList();
//}

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
    for (int i = 1; i < rm.getPath().length(); i++) {
        tmp["origin"] = QVariant::fromValue(rm.getPath()[i - 1]);
        tmp["destination"] = QVariant::fromValue(rm.getPath()[i]);
        m_coordinateList.push_back(tmp);
    }
    foreach (QGeoManeuver maneuver, rm.getManeuver()) {
        qDebug() << maneuver.instructionText() << maneuver.distanceToNextInstruction();
    }
    emit updateRoutePlanning();
    emit updateManeuver(rm.getManeuver());
}
void MapManager::updateRoute(double time, int step)
{
    m_coordinateList.clear();
    QVariantList routes = DataBase::dataBase.getRoute(time, step);
    routeCount = routes.length();
    foreach (QVariant route, routes) {
        rm.setRoute(route.toMap()["origin"].value<QGeoCoordinate>(),
                    route.toMap()["destination"].value<QGeoCoordinate>(),
                    1);
    }
}

void MapManager::updatedRoute()
{
    //    m_coordinateList.push_back(QVariant::fromValue(rm.getPath()));
    QVariantMap tmp;
    //    tmp["routes"] = QVariant::fromValue(rm.getRoute());
    QVariantList list;
    foreach (QGeoCoordinate geo, rm.getPath()) {
        list.push_back(QVariant::fromValue(geo));
    }
    tmp["paths"] = list;
    m_coordinateList.push_back(tmp);
    qDebug() << "updated" << routeCount;
    routeCount--;
    if (routeCount % 10 == 0)
        emit updateCoordinateList();
}

void MapManager::predictRoute()
{
    m_coordinateList = DataBase::dataBase.getRelateTime(coordinateFrom(), coordinateTo());
    emit updateDots();
    emit updateRouteIdeas(m_coordinateList);
    qDebug() << "updated";
}

void MapManager::predictSpace(int time)
{
    m_coordinateList = DataBase::dataBase.getRelateSpace(coordinateFrom(), time);
    emit updateDots();
    emit updateSpaceIdeas(m_coordinateList);
}
