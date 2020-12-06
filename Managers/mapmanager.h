#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "Utilities/database.h"
#include <QGeoCoordinate>
#include <QObject>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickWidget>

class MapManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList gridList READ gridList NOTIFY updateGridList);
    Q_PROPERTY(QVariantList coordinateList READ coordinateList WRITE setCoordinate NOTIFY
                   updateCoordinateList);
    Q_PROPERTY(QGeoCoordinate coordinateFrom READ coordinateFrom WRITE setCoordinateFrom NOTIFY
                   updateCoordinateFrom);
    Q_PROPERTY(QGeoCoordinate coordinateTo READ coordinateTo WRITE setCoordinateTo NOTIFY
                   updateCoordinateTo);

public:
    static MapManager *getManager(QQuickWidget *root, QString id = "mapManager");
    explicit MapManager(QObject *parent = nullptr);
    QVariantList gridList() const;
    QVariantList coordinateList() const;
    QGeoCoordinate coordinateFrom() const;
    QGeoCoordinate coordinateTo() const;
    void setCoordinateList(const QVariantList &coordinateList);
    void setCoordinateFrom(const QGeoCoordinate &coordinateFrom);
    void setCoordinateTo(const QGeoCoordinate &coordinateTo);
    void initGrids();
    void updateHeat(double start, double end);

private:
    QQuickWidget *root;
    QString id;
    QObject *model;
    QVariantList m_gridList;
    QPair<QGeoCoordinate, QGeoCoordinate> m_coordinateFromTo;
    QVariantList m_coordinateList;
signals:
    void updateGrid();
    void updateHeat();
    void updateGridList();
    void updateCoordinateList();
    void updateCoordinateFrom();
    void updateCoordinateTo();
};

#endif // MAPMANAGER_H
