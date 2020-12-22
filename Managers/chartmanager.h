#ifndef CHARTMANAGER_H
#define CHARTMANAGER_H

#include "Managers/axismanager.h"
#include "Managers/seriesmanager.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QObject>
#include <QtCharts>

class ChartManager : public QObject
{
    Q_OBJECT
public:
    //    enum ChartType { Spline, Line, Scatter };
    explicit ChartManager(QGridLayout *ctn = nullptr, QString ttl = NULL, QObject *parent = nullptr);
    SeriesManager *getSeries() { return series; }
    inline void addSeriesId(QString id, QStringList recodes,
                            QMap<QString, SeriesManager::SeriesType> typeConfig = {})
    {
        seriesConfig.idFields.push_back(id);
        seriesConfig.recodeFields[id] = recodes;
        foreach (QString recode, typeConfig.keys()) {
            seriesConfig.typeConfig[recode] = typeConfig[recode];
        }
        series->reset(seriesConfig);
    }
    void initAxisX(AxisManager::AxisType type, Qt::Alignment alignment,
                   QMap<AxisManager::AxisConfigItem, QVariant> config = {});
    void initAxisY(AxisManager::AxisType type, Qt::Alignment alignment,
                   QMap<AxisManager::AxisConfigItem, QVariant> config = {});
    void addAxis(QString name, AxisManager::AxisType type, Qt::Alignment alignment,
                 QStringList seriesList = {},
                 QMap<AxisManager::AxisConfigItem, QVariant> config = AxisManager::internal_value);
    inline AxisManager *getAxisX() { return axisX; }
    inline AxisManager *getAxisY() { return axisY; }
    inline AxisManager *getOtherAxis(QString field) { return otherAxis[field]; }
    ~ChartManager();
public slots:
    void load();
signals:
private:
    //    ChartType type;
    QString title;
    SeriesManager::SeriesConfig seriesConfig;
    QGridLayout *container;
    //    QList<QXYSeries *> serieses;
    QChart *chart;
    QChartView *chartView;
    AxisManager *axisX;
    AxisManager *axisY;
    QMap<QString, AxisManager *> otherAxis;
    QMap<QString, QStringList> otherAxisSeries;
    SeriesManager *series;
};

#endif // CHARTMANAGER_H
