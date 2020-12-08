#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QMap>
#include <QObject>
#include <QVariant>

class GlobalData : public QObject
{
    Q_OBJECT
private:
    QMap<QString, QVariant> config;
    QMap<QString, QVariant> environment;
    static double lng_min;
    static double lng_max;
    static double lat_min;
    static double lat_max;

public:
    static GlobalData globalData;
    GlobalData();
    void conf_set(QString key, QVariant value);
    QVariant conf_get(QString key) const;
    void env_set(QString key, QVariant value);
    QVariant env_get(QString key) const;
    static int get_grid(int step, double lng, double lat);
    static double get_edge_lng(int step, int index);
    static double get_edge_lat(int step, int index);
};
#endif // GLOBALDATA_H
