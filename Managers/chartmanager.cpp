#include "chartmanager.h"

ChartManager::ChartManager(QGridLayout *ctn, QString ttl, QObject *parent)
    : QObject(parent), title(ttl), container(ctn)
{
    series = new SeriesManager;
    axisX = new ValueAxisManager(Qt::AlignBottom);
    axisX->loadConfig(AxisManager::internal_value);
    axisY = new ValueAxisManager(Qt::AlignLeft);
    axisY->loadConfig(AxisManager::internal_value);
}
void ChartManager::load()
{
    qDebug() << "Start to load the chart ...";
    qDebug() << "Clear the old data ...";
    if (container->widget() != NULL) {
        container->removeWidget(container->widget());
        delete chart;
        delete chartView;
    }
    qDebug() << "Recreate the data ...";
    series->reset(seriesConfig);
    //    foreach (QPointF p, series->getSeries(series->fieldList()[0])->points())
    //        qDebug() << QDateTime::fromTime_t(p.x());
    qDebug() << "Setting the axes ...";
    chart = new QChart;
    axisX->addAxis(chart);
    axisY->addAxis(chart);
    QStringList otherSeriesList;
    foreach (QString field, otherAxisSeries.keys()) {
        otherSeriesList.append(otherAxisSeries[field]);
    }
    foreach (QString id, series->idList()) {
        foreach (QString field, series->fieldList(id)) {
            chart->addSeries(series->getSeries(field));
            if (!otherSeriesList.contains(field)) {
                axisX->attachSeries(series->getSeries(field));
                axisY->attachSeries(series->getSeries(field));
            }
        }
    }
    foreach (QString field, otherAxis.keys()) {
        otherAxis[field]->addAxis(chart);
        foreach (QString str, otherAxisSeries[field])
            otherAxis[field]->attachSeries(series->getSeries(str));
    }
    qDebug() << "Loading the chart ...";
    if (!title.isNull())
        chart->setTitle(title);
    chartView = new QChartView(chart);
    qDebug() << "Setting the chartView ...";
    QSizePolicy policy = chartView->sizePolicy();
    policy.setVerticalPolicy(QSizePolicy::Expanding);
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    chartView->setSizePolicy(policy);
    chartView->setRenderHint(QPainter::Antialiasing);
    container->addWidget(chartView, 0, 0);
    qDebug() << "Loading finished ...";
}

void ChartManager::initAxisX(AxisManager::AxisType type, Qt::Alignment alignment,
                             QMap<AxisManager::AxisConfigItem, QVariant> config)
{
    if (!(axisX == nullptr))
        delete axisX;
    switch (type) {
    case AxisManager::AxisType::DataTime:
        axisX = new DateTimeAxisManager(alignment);
        break;
    case AxisManager::AxisType::Value:
        axisX = new ValueAxisManager(alignment);
        break;
    default:
        assert(0);
    }
    axisX->loadConfig(config);
}

void ChartManager::initAxisY(AxisManager::AxisType type, Qt::Alignment alignment,
                             QMap<AxisManager::AxisConfigItem, QVariant> config)
{
    if (!(axisY == nullptr))
        delete axisY;
    switch (type) {
    case AxisManager::AxisType::DataTime:
        axisY = new DateTimeAxisManager(alignment);
        break;
    case AxisManager::AxisType::Value:
        axisY = new ValueAxisManager(alignment);
        break;
    default:
        assert(0);
    }
    axisX->loadConfig(config);
}

void ChartManager::addAxis(QString name, AxisManager::AxisType type, Qt::Alignment alignment,
                           QStringList seriesList,
                           QMap<AxisManager::AxisConfigItem, QVariant> config)
{
    if (otherAxis.contains(name)) {
        delete otherAxis[name];
    }
    switch (type) {
    case AxisManager::AxisType::DataTime:
        otherAxis[name] = new DateTimeAxisManager(alignment);
        break;
    case AxisManager::AxisType::Value:
        otherAxis[name] = new ValueAxisManager(alignment);
        break;
    default:
        assert(0);
    }
    otherAxis[name]->loadConfig(config);
    if (seriesList.isEmpty())
        otherAxisSeries[name] = QStringList({name});
    else
        otherAxisSeries[name] = seriesList;
}

ChartManager::~ChartManager()
{
    delete series;
    delete axisX;
    delete axisY;
    delete chart;
    delete chartView;
}
