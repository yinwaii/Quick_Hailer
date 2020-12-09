#include "routemanager.h"

RouteManager::RouteManager() : provider("osm"), routingManager(provider.routingManager())
{
    connect(routingManager,
            SIGNAL(finished(QGeoRouteReply *)),
            this,
            SLOT(routeCalculated(QGeoRouteReply *)));

    connect(routingManager,
            SIGNAL(error(QGeoRouteReply *, QGeoRouteReply::Error, QString)),
            this,
            SLOT(routeError(QGeoRouteReply *, QGeoRouteReply::Error, QString)));
}

void RouteManager::setRoute(const QGeoCoordinate &origin, const QGeoCoordinate &destination)
{
    QGeoRouteRequest request(origin, destination);

    request.setTravelModes(QGeoRouteRequest::CarTravel);
    request.setRouteOptimization(QGeoRouteRequest::MostEconomicRoute);

    QGeoRouteReply *reply = routingManager->calculateRoute(request);
    if (reply->isFinished()) {
        if (reply->error() == QGeoRouteReply::NoError) {
            routeCalculated(reply);
            route = reply->routes();
        } else {
            routeError(reply, reply->error(), reply->errorString());
        }
        return;
    }
}

QList<QGeoCoordinate> RouteManager::getPath() const
{
    return route.at(0).path();
}

int RouteManager::getTravelTime() const
{
    return route.at(0).travelTime();
}

QList<QGeoManeuver> RouteManager::getManeuver() const
{
    QList<QGeoManeuver> tmp;
    for (QGeoRouteSegment i = route.at(0).firstRouteSegment(); i.isValid();
         i = i.nextRouteSegment()) {
        tmp.push_back(i.maneuver());
    }
    return tmp;
}

void RouteManager::routeCalculated(QGeoRouteReply *reply)
{
    // A route request can ask for several alternative routes ...
    if (reply->routes().size() != 0) {
        // ... but by default it will only get a single route
        //        QGeoRoute route = reply->routes().at(0);
        route = reply->routes();
        qDebug() << "calculated" << reply->routes().length();
        emit updateLine();

        //... now we have to make use of the route ...
    }

    reply->deleteLater();
}

void RouteManager::routeError(QGeoRouteReply *reply, QGeoRouteReply ::Error error,
                              const QString &errorString)
{
    // ... inform the user that an error has occurred ...
    reply->deleteLater();
}
