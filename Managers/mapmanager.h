#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "Utilities/database.h"
#include <QObject>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickWidget>

class MapManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList grids READ grids WRITE setGrids NOTIFY gridsChanged)
    Q_PROPERTY(QVariantList heat READ heat WRITE setHeat NOTIFY heatChanged)
    Q_PROPERTY(QVariantList gridData READ gridData WRITE setGridData NOTIFY gridDataChanged)
public:
    static MapManager *getMapManager(QQuickWidget *widget);
    explicit MapManager(QObject *parent = nullptr);
    void setRoot(QQuickWidget *item);
    QVariantList grids() const;
    QVariantList heat() const;
    QVariantList gridData() const;
    void setGrids(const QVariantList &grids);
    void setHeat(const QVariantList &heat);
    void setGridData(const QVariantList &gridData);
    Q_INVOKABLE void initGrids();
    Q_INVOKABLE void updateHeat(double start, double end);
    Q_INVOKABLE void updateModel();

private:
    QQuickWidget *root;
    QVariantList m_gridData;
signals:
    void gridsChanged();
    void heatChanged();
    void gridDataChanged();
};

#endif // MAPMANAGER_H
