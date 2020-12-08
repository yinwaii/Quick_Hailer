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

MapManager::MapManager(QObject *parent) : QObject(parent)
{
    //    assert(widget != nullptr);
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
