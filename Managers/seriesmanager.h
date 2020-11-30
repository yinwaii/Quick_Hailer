#ifndef SERIESMANAGER_H
#define SERIESMANAGER_H

#include <QMap>
#include <QObject>
#include <QPair>
#include <QString>
#include <QtCharts>

class SeriesManager : public QObject
{
    Q_OBJECT
private:
    QMap<QString, QXYSeries *> serieses;
    QMap<QString, QList<QPointF>> cache;
    QMap<QString, QPair<double, double>> min_max;
    QMap<QString, QPair<double, double>> min_max_cache;
    QPair<QString, QStringList> fields;

public:
    enum SeriesType { SplineSeries, LineSeries, ScatterSeries };
    explicit SeriesManager(QString idField = "", QStringList recodeFields = {},
                           QObject *parent = nullptr);
    void setType(QMap<QString, SeriesType> typeConfig);
    void addCache(double id, QList<double> recodes);
    void loadRecodes();
    void clearRecode(QString field);
    void clearAllRecode();
    void reset(QString idField, QStringList recodeFields);
    inline QString idField() const { return fields.first; }
    inline QStringList fieldList() const { return fields.second; }
    inline QXYSeries *getSeries(QString field) const { return serieses[field]; }
    inline QPair<double, double> getBound(QString field) const { return min_max[field]; }
    ~SeriesManager();
signals:

};

#endif // SERIESMANAGER_H
