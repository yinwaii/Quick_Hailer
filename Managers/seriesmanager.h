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
    QMap<QString, QStringList> fields;

public:
    enum SeriesType { SplineSeries, LineSeries, ScatterSeries };
    struct SeriesConfig
    {
        QStringList idFields;
        QMap<QString, QStringList> recodeFields;
        QMap<QString, SeriesType> typeConfig;
    };
    explicit SeriesManager(QString idField = "", QStringList recodeFields = {},
                           QObject *parent = nullptr);
    void addId(QString idField, QStringList recodeFields);
    void setType(QMap<QString, SeriesType> typeConfig);
    void addCache(QString idField, double id, QList<double> recodes);
    void loadRecodes();
    void clearRecode(QString id, QString field);
    void clearAllRecode();
    void reset(QStringList idFields, QMap<QString, QStringList> recodeFields,
               QMap<QString, SeriesManager::SeriesType> typeConfig = {});
    inline void reset(SeriesConfig seriesconfig)
    {
        reset(seriesconfig.idFields, seriesconfig.recodeFields, seriesconfig.typeConfig);
    }
    inline QStringList idList() const { return fields.keys(); }
    inline QStringList fieldList(QString id) const { return fields[id]; }
    inline QXYSeries *getSeries(QString field) const { return serieses[field]; }
    inline QPair<double, double> getBound(QString field) const { return min_max[field]; }
    ~SeriesManager();
signals:

};

#endif // SERIESMANAGER_H
