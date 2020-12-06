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

void MapManager::updateHeat(double start, double end)
{
    int maxEntry = -1, maxExit = -1;
    for (int i = 0; i < m_gridList.count(); i++) {
        QRectF grid = m_gridList[i].toMap()["grid"].toRectF();
        int entry = DataBase::dataBase.searchNum(
            QString("SELECT * from dataset WHERE departure_time > %1 AND departure_time < %2 AND "
                    "orig_lng "
                    "> %3 AND orig_lng < %4 AND orig_lat > %5 AND orig_lat < %6")
                .arg(start)
                .arg(end)
                .arg(grid.topLeft().ry())
                .arg(grid.bottomRight().ry())
                .arg(grid.bottomRight().rx())
                .arg(grid.topLeft().rx()));
        int exit = DataBase::dataBase.searchNum(
            QString("SELECT * from dataset WHERE end_time > %1 AND end_time < %2 AND "
                    "dest_lng "
                    "> %3 AND dest_lng < %4 AND dest_lat > %5 AND dest_lat < %6")
                .arg(start)
                .arg(end)
                .arg(grid.topLeft().ry())
                .arg(grid.bottomRight().ry())
                .arg(grid.bottomRight().rx())
                .arg(grid.topLeft().rx()));
        QVariantMap tmp = m_gridList[i].toMap();
        tmp["entry"] = entry;
        tmp["exit"] = exit;
        m_gridList[i] = tmp;
        if (maxEntry == -1 || entry > maxEntry)
            maxEntry = entry;
        if (maxExit == -1 || exit > maxExit)
            maxExit = exit;
        //        emit dataChanged(index(i), index(i), QVector<int>() << EntryRole << ExitRole);
    }
    for (int i = 0; i < m_gridList.count(); i++) {
        QVariantMap tmp = m_gridList[i].toMap();
        tmp["entry"] = double(m_gridList[i].toMap()["entry"].toInt()) / double(maxEntry);
        tmp["exit"] = double(m_gridList[i].toMap()["exit"].toInt()) / double(maxExit);
        m_gridList[i] = tmp;
        qDebug() << m_gridList[i];
    }
    emit updateHeat();
    emit updateGridList();
}
