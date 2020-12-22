#ifndef ROUTEMANAGER_H
#define ROUTEMANAGER_H

#include <QDebug>
#include <QGeoAddress>
#include <QGeoCodingManager>
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
    void setRoute(const QGeoCoordinate &origin, const QGeoCoordinate &destination, int option = 0);
    void setCoordinate(const QGeoCoordinate &coordinate, int option = 0);
    QList<QGeoCoordinate> getPath() const;
    QList<QGeoManeuver> getManeuver() const;
    int getTravelTime() const;
    QString getAddress() const;
    QGeoRoute getRoute() const;

private:
    QGeoServiceProvider provider;
    QGeoRoutingManager *routingManager;
    QGeoCodingManager *codingManager;
    QList<QGeoRoute> route;
    QList<QGeoLocation> addresses;
    int m_option;
    int p_option;
private slots:
    void routeCalculated(QGeoRouteReply *reply);
    void addressFetched(QGeoCodeReply *reply);
    void routeError(QGeoRouteReply *reply, QGeoRouteReply::Error error, const QString &errorString);
    void addressError(QGeoCodeReply *reply, QGeoCodeReply::Error error, const QString &errorString);
signals:
    void updateLine();
    void updateFlow();
    void updateOrigin();
    void updateDestination();
};

#endif // ROUTEMANAGER_H
