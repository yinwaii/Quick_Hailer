#include "routemanager.h"

RouteManager::RouteManager()
    : provider("osm"), routingManager(provider.routingManager()),
      codingManager(provider.geocodingManager())
{
    connect(routingManager, &QGeoRoutingManager::finished, this, &RouteManager::routeCalculated);

    connect(routingManager, &QGeoRoutingManager::error, this, &RouteManager::routeError);
    connect(codingManager, &QGeoCodingManager::finished, this, &RouteManager::addressFetched);
    connect(codingManager, &QGeoCodingManager::error, this, &RouteManager::addressError);
}

void RouteManager::setRoute(const QGeoCoordinate &origin, const QGeoCoordinate &destination,
                            int option)
{
    m_option = option;
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

void RouteManager::setCoordinate(const QGeoCoordinate &coordinate, int option)
{
    p_option = option;
    QGeoCodeReply *reply = codingManager->reverseGeocode(coordinate);
    if (reply->isFinished()) {
        if (reply->error() == QGeoCodeReply::NoError) {
            addressFetched(reply);
            addresses = reply->locations();
        } else {
            addressError(reply, reply->error(), reply->errorString());
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
        switch (m_option) {
        case 0:
            emit updateLine();
            break;
        case 1:
            emit updateFlow();
            break;
        }

        //... now we have to make use of the route ...
    }

    reply->deleteLater();
}

void RouteManager::routeError(QGeoRouteReply *reply, QGeoRouteReply ::Error error,
                              const QString &errorString)
{
    // ... inform the user that an error has occurred ...
    qDebug() << "error:" << error;
    reply->deleteLater();
}

void RouteManager::addressFetched(QGeoCodeReply *reply)
{
    if (reply->locations().size() != 0) {
        addresses = reply->locations();
        qDebug() << "fetched" << reply->locations().length();
        switch (p_option) {
        case 0:
            emit updateOrigin();
            break;
        case 1:
            emit updateDestination();
            break;
        }
    }
    reply->deleteLater();
}

void RouteManager::addressError(QGeoCodeReply *reply, QGeoCodeReply::Error error,
                                const QString &errorString)
{
    qDebug() << "error:" << error;
    reply->deleteLater();
}

QGeoRoute RouteManager::getRoute() const
{
    return route.at(0);
}

QString RouteManager::getAddress() const
{
    qDebug() << addresses.at(0).address().text();
    QStringList addressText = addresses.at(0).address().text().split(", ");
    return QString("%1(%2)").arg(addressText[0]).arg(addressText[1]);
}
