#include "mapmanager.h"

MapManager::MapManager(QObject *parent) : QObject(parent)
{

}

void MapManager::initGrids()
{
    //    qDebug() << DataBase::dataBase.getGrid();
    //        qDebug() << m_grids;
    m_grids = DataBase::dataBase.getGrid();
    emit gridsChanged();
}

QVariantList MapManager::grids() const
{
    return m_grids;
}

void MapManager::setGrids(const QVariantList &grids)
{
    if (m_grids != grids) {
        m_grids = grids;
        emit gridsChanged();
    }
}
