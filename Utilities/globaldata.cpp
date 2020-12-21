#include "globaldata.h"

GlobalData GlobalData::globalData;
double GlobalData::lng_min = 103.90840747453100;
double GlobalData::lng_max = 104.22204452546901;
double GlobalData::lat_min = 30.524081949676100;
double GlobalData::lat_max = 30.793878050323901;

GlobalData::GlobalData()
{

}

void GlobalData::env_set(QString key, QVariant value)
{
    environment[key] = value;
}

QVariant GlobalData::env_get(QString key) const
{
    return environment[key];
}

void GlobalData::conf_set(QString key, QVariant value)
{
    config[key] = value;
}

QVariant GlobalData::conf_get(QString key) const
{
    return config[key];
}

int GlobalData::get_grid(int step, double lng, double lat)
{
    double lngStepLength = (lng_max - lng_min) / double(step);
    int lngIndex = floor((lng - lng_min) / lngStepLength);
    double latStepLength = (lat_max - lat_min) / double(step);
    int latIndex = floor((lat - lat_min) / latStepLength);
    int index = lngIndex + step * latIndex;
    return index;
}

double GlobalData::get_edge_lng(int step, int index)
{
    double lngStepLength = (lng_max - lng_min) / double(step);
    return lng_min + lngStepLength * index;
}

double GlobalData::get_edge_lat(int step, int index)
{
    double latStepLength = (lat_max - lat_min) / double(step);
    return lat_min + latStepLength * index;
}

QGeoCoordinate GlobalData::get_coordinate(int step, int index)
{
    QGeoCoordinate result;
    result.setLongitude((get_edge_lng(step, index % step) + get_edge_lng(step, (index % step) + 1))
                        / 2);
    result.setLatitude((get_edge_lat(step, index / step) + get_edge_lat(step, (index / step) + 1))
                       / 2);
    return result;
}

int GlobalData::get_step(int step, double time, double start, double stop)
{
    double timeStepLength = (stop - start) / double(step);
    int index = floor((time - start) / timeStepLength);
    return index;
}

double GlobalData::get_time(int step, int index, double start, double stop)
{
    double timeStepLength = (stop - start) / double(step);
    return start + timeStepLength * index;
}
