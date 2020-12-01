#include "chartmanager.h"

ChartManager::ChartManager(QGridLayout *ctn, QObject *parent) : QObject(parent), container(ctn)
{
    series = new SeriesManager;
    axisX = new DateTimeAxisManager(Qt::AlignBottom);
    axisX->loadConfig(AxisManager::internal_date);
    axisY = new ValueAxisManager(Qt::AlignLeft);
    axisY->loadConfig(AxisManager::internal_value);
}
void ChartManager::load()
{
    //    qDebug() << data;
    //    assert(!data.isEmpty());
    //    assert(data[0].size() >= 2);
    if (container->widget() != NULL) {
        container->removeWidget(container->widget());
        //        foreach (QXYSeries *series, serieses)
        //            delete series;
        //        serieses.clear();
        delete chart;
        delete chartView;
    }
    series->clearAllRecode();
    series->reset(seriesConfig.idField, seriesConfig.recodeFields);
    series->setType(seriesConfig.typeConfig);
    series->loadRecodes();

    //    foreach (QList<QPointF> dat, data) {
    //        //        qDebug() << dat;
    //        QXYSeries *series;
    //        switch (type) {
    //        case Spline:
    //            series = new QSplineSeries;
    //            break;
    //        case Line:
    //            series = new QLineSeries;
    //            break;
    //        case Scatter:
    //            series = new QScatterSeries;
    //            break;
    //        default:
    //            QMessageBox::warning(nullptr, "error", "Invalid Chart Type!");
    //            assert(0);
    //        }
    //        series->replace(dat);
    //        serieses.push_back(series);
    //    }
    //    axisY->setRange(series->getBound(series->fieldList()[0]));
    //    axisX->loadRange();
    //    foreach (QPointF p, series->getSeries(series->fieldList()[0])->points())
    //        qDebug() << QDateTime::fromTime_t(p.x());
    chart = new QChart;
    axisX->addAxis(chart);
    axisY->addAxis(chart);
    foreach (QString field, series->fieldList()) {
        chart->addSeries(series->getSeries(field));
        axisX->attachSeries(series->getSeries(field));
        axisY->attachSeries(series->getSeries(field));
    }
    //    foreach (QXYSeries *series, serieses) {
    //        chart->addSeries(series);
    //    }
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    container->addWidget(chartView, 0, 0);
}

ChartManager::~ChartManager()
{
    delete series;
    delete axisX;
    delete axisY;
    delete chart;
    delete chartView;
}
