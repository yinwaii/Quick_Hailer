#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "Utilities/database.h"
#include <QObject>

class MapManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList grids READ grids WRITE setGrids NOTIFY gridsChanged)
public:
    explicit MapManager(QObject *parent = nullptr);
    QVariantList grids() const;
    void setGrids(const QVariantList &grids);
    Q_INVOKABLE void initGrids();

private:
    QVariantList m_grids;
signals:
    void gridsChanged();
};

#endif // MAPMANAGER_H
