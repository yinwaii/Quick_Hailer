#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "Managers/routemanager.h"
#include "Utilities/database.h"
#include "Utilities/globaldata.h"
#include <QGeoCoordinate>
#include <QGeoPath>
#include <QObject>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickWidget>

class MapManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList gridList READ gridList NOTIFY updateGridList);
    Q_PROPERTY(QVariantList coordinateList READ coordinateList WRITE setCoordinateList NOTIFY
                   updateCoordinateList);
    Q_PROPERTY(QGeoCoordinate coordinateFrom READ coordinateFrom WRITE setCoordinateFrom NOTIFY
                   updateCoordinateFrom);
    Q_PROPERTY(QGeoCoordinate coordinateTo READ coordinateTo WRITE setCoordinateTo NOTIFY
                   updateCoordinateTo);
    Q_PROPERTY(int selectStatus READ selectStatus WRITE setSelectStatus NOTIFY updateSelectStatus);

public:
    static MapManager *getManager(QQuickWidget *root, QString id = "mapManager");
    explicit MapManager(QObject *parent = nullptr);
    QVariantList gridList() const;
    QVariantList coordinateList() const;
    QGeoCoordinate coordinateFrom() const;
    QGeoCoordinate coordinateTo() const;
    int selectStatus() const;
    void setCoordinateList(const QVariantList &coordinateList);
    void setCoordinateFrom(const QGeoCoordinate &coordinateFrom);
    void setCoordinateTo(const QGeoCoordinate &coordinateTo);
    void setSelectStatus(const int &status);
    void initGrids();
    void updateHeatEntry(double start, double end, int step);
    void updateHeatExit(double start, double end, int step);
    void updateRoute(double time, int step);
    Q_INVOKABLE void addGrid(int grid);
    Q_INVOKABLE void deleteGrid(int grid);

private:
    int routeCount;
    QQuickWidget *root;
    QString id;
    RouteManager rm;
    QObject *model;
    QVariantList m_gridList;
    QPair<QGeoCoordinate, QGeoCoordinate> m_coordinateFromTo;
    QVariantList m_coordinateList;
    QList<int> m_gridSelected;
    int m_selectStatus;
public slots:
    void selectFrom();
    void selectTo();
    void selectPlanning();
    void selectedPlanning();
    void updatedRoute();
    void predictRoute();
    void predictSpace(int time);
signals:
    void updateGrid();
    void updateHeatEntry();
    void updateHeatExit();
    void updateGridList();
    void updateCoordinateList();
    void updateCoordinateFrom();
    void updateCoordinateTo();
    void updateSelectStatus();
    void updateRoutePlanning();
    void updateManeuver(QList<QGeoManeuver> maneuver);
    void updateDots();
    void updateRouteIdeas(QVariantList ideaList);
    void updateSpaceIdeas(QVariantList ideaList);
    void updateDemandPlot();
};

#endif // MAPMANAGER_H
