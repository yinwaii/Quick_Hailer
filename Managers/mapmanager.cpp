#include "mapmanager.h"

MapManager *MapManager::getMapManager(QQuickWidget *widget)
{
    MapManager *manager = widget->rootObject()->findChild<MapManager *>("mapManager");
    manager->setRoot(widget);
    return manager;
}

MapManager::MapManager(QObject *parent) : QObject(parent)
{

}

void MapManager::setRoot(QQuickWidget *item)
{
    root = item;
}

void MapManager::initGrids()
{
    //    qDebug() << DataBase::dataBase.getGrid();
    //        qDebug() << m_grids;
    m_gridData = DataBase::dataBase.getGrid();
    updateModel();
    emit gridsChanged();
    emit gridDataChanged();
}

QVariantList MapManager::grids() const
{
    QVariantList tmp;
    foreach (QVariant gridItem, m_gridData)
        tmp.push_back(gridItem.toMap()["grid"]);
    return tmp;
}

void MapManager::setGrids(const QVariantList &n_grids)
{
    if (grids() != n_grids) {
        m_gridData.clear();
        foreach (QVariant grid, n_grids) {
            QVariantMap tmp;
            tmp["grid"] = grid;
            m_gridData.push_back(tmp);
        }
        emit gridsChanged();
        emit gridDataChanged();
    }
}

void MapManager::updateHeat(double start, double end)
{
    for (int i = 0; i < m_gridData.size(); i++) {
        QRectF grid = m_gridData[i].toMap()["grid"].toRectF();
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
        m_gridData[i].toMap()["heat"] = QPoint({entry, exit});
    }
    updateModel();
    emit heatChanged();
    emit gridDataChanged();
}

QVariantList MapManager::heat() const
{
    QVariantList tmp;
    foreach (QVariant gridItem, m_gridData) {
        tmp.push_back(gridItem.toMap()["heat"]);
    }
    return tmp;
}

void MapManager::setHeat(const QVariantList &n_heat)
{
    if (heat() != n_heat && m_gridData.size() == n_heat.size()) {
        for (int i = 0; i < m_gridData.size(); i++) {
            m_gridData[i].toMap()["heat"] = n_heat[i];
        }
        emit heatChanged();
        emit gridDataChanged();
    }
}

void MapManager::updateModel()
{
    root->rootContext()->setContextProperty("myGridModel", QVariant::fromValue(m_gridData));
}

QVariantList MapManager::gridData() const
{
    return m_gridData;
}

void MapManager::setGridData(const QVariantList &gridData)
{
    if (m_gridData != gridData) {
        m_gridData = gridData;
        emit gridDataChanged();
    }
}
