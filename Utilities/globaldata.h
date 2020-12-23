#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QDebug>
#include <QGeoCoordinate>
#include <QMap>
#include <QObject>
#include <QThread>
#include <QVariant>

class GlobalData : public QObject
{
    Q_OBJECT
private:
    QList<QThread *> threads;
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
    static QGeoCoordinate get_coordinate(int step, int index);
    static int get_step(int step, double time, double start, double stop);
    static double get_time(int step, int index, double start, double stop);
    void clear_threads();
    void remove_threads(QThread *thread);
    bool isEmpty_threads();
    void add_threads(QThread *thread);
};
#endif // GLOBALDATA_H
