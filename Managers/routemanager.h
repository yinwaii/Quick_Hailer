#ifndef ROUTEMANAGER_H
#define ROUTEMANAGER_H

#include <QDebug>
#include <QGeoManeuver>
#include <QGeoRouteSegment>
#include <QGeoRoutingManager>
#include <QGeoServiceProvider>
#include <QObject>

class RouteManager : public QObject
{
    Q_OBJECT
public:
    RouteManager();
    void setRoute(const QGeoCoordinate &origin, const QGeoCoordinate &destination);
    QList<QGeoCoordinate> getPath() const;
    QList<QGeoManeuver> getManeuver() const;
    int getTravelTime() const;

private:
    QGeoServiceProvider provider;
    QGeoRoutingManager *routingManager;
    QList<QGeoRoute> route;
private slots:
    void routeCalculated(QGeoRouteReply *reply);
    void routeError(QGeoRouteReply *reply, QGeoRouteReply::Error error, const QString &errorString);
signals:
    void updateLine();
};

#endif // ROUTEMANAGER_H
