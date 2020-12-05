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
    Q_PROPERTY(QVariantList gridList READ gridList NOTIFY updateGridList);

public:
    static MapManager *getManager(QQuickWidget *root, QString id = "mapManager");
    explicit MapManager(QObject *parent = nullptr);
    QVariantList gridList() const;
    void initGrids();
    void updateHeat(double start, double end);

private:
    QQuickWidget *root;
    QString id;
    QObject *model;
    QVariantList m_gridList;
signals:
    void updateGrid();
    void updateHeat();
    void updateGridList();
};

#endif // MAPMANAGER_H
