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
    explicit ChartManager(QGridLayout *ctn = nullptr, QObject *parent = nullptr);
    void load();
    SeriesManager *getSeries() { return series; }
    inline void initSeries(QString id, QStringList recodes,
                           QMap<QString, SeriesManager::SeriesType> typeConfig = {})
    {
        seriesConfig.idField = id;
        seriesConfig.recodeFields = recodes;
        seriesConfig.typeConfig = typeConfig;
        series->clearAllRecode();
        series->reset(seriesConfig.idField, seriesConfig.recodeFields);
        series->setType(seriesConfig.typeConfig);
    }
    inline AxisManager *getAxisX() { return axisX; }
    inline AxisManager *getAxisY() { return axisY; }
    ~ChartManager();
signals:
private:
    //    ChartType type;
    struct
    {
        QString idField;
        QStringList recodeFields;
        QMap<QString, SeriesManager::SeriesType> typeConfig;
    } seriesConfig;
    QGridLayout *container;
    //    QList<QXYSeries *> serieses;
    QChart *chart;
    QChartView *chartView;
    AxisManager *axisX;
    AxisManager *axisY;

    SeriesManager *series;
};

#endif // CHARTMANAGER_H
