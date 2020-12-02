#ifndef AXISMANAGER_H
#define AXISMANAGER_H

#include <QObject>
#include <QtCharts>

class AxisManager : public QObject
{
    Q_OBJECT

public:
    enum AxisType { DataTime, Value };
    enum AxisConfigItem { TickCount, Format, LabelsAngle, Range };
    static QMap<AxisConfigItem, QVariant> internal_date;
    static QMap<AxisConfigItem, QVariant> internal_value;
    AxisManager(Qt::Alignment alignment) : align(alignment) { init(); }
    virtual void setTickCount(int num) = 0;
    virtual void setFormat(QString format) = 0;
    virtual void setLabelsAngle(int angle) = 0;
    virtual void addAxis(QChart *chart) = 0;
    virtual void attachSeries(QXYSeries *series) = 0;
    inline void setRange(QPair<double, double> minmax) { bound = minmax; }
    inline void setRange(double min, double max) { bound = qMakePair(min, max); }
    virtual void loadRange() = 0;
    inline void setAlignment(Qt::Alignment alignment) { align = alignment; }
    void loadConfig();
    inline void loadConfig(const QMap<AxisConfigItem, QVariant> internal_config)
    {
        config = internal_config;
        loadConfig();
        config.clear();
    }
    virtual ~AxisManager() {}

protected:
    static void init();
    Qt::Alignment align;
    QPair<double, double> bound;
    QMap<AxisConfigItem, QVariant> config;

signals:
};

class DateTimeAxisManager : public AxisManager
{
    Q_OBJECT
private:
    QDateTimeAxis *axis;

public:
    DateTimeAxisManager(Qt::Alignment alignment) : AxisManager(alignment)
    {
        axis = new QtCharts::QDateTimeAxis;
    }
    inline void setTickCount(int num) override { axis->setTickCount(num); }
    inline void setFormat(QString format) override { axis->setFormat(format); }
    inline void setLabelsAngle(int angle) override { axis->setLabelsAngle(angle); }
    inline void addAxis(QChart *chart) override { chart->addAxis(axis, align); }
    inline void attachSeries(QXYSeries *series) override { series->attachAxis(axis); }
    inline void loadRange() override
    {
        axis->setRange(QDateTime::fromMSecsSinceEpoch(int(bound.first)),
                       QDateTime::fromMSecsSinceEpoch(int(bound.second)));
    }
    ~DateTimeAxisManager() { delete axis; }
};

class ValueAxisManager : public AxisManager
{
    Q_OBJECT
private:
    QValueAxis *axis;

public:
    ValueAxisManager(Qt::Alignment alignment) : AxisManager(alignment)
    {
        axis = new QtCharts::QValueAxis;
    }
    inline void setTickCount(int num) override { axis->setTickCount(num); }
    inline void setFormat(QString format) override { axis->setLabelFormat(format); }
    inline void setLabelsAngle(int angle) override { axis->setLabelsAngle(angle); }
    inline void addAxis(QChart *chart) override { chart->addAxis(axis, align); }
    inline void attachSeries(QXYSeries *series) override { series->attachAxis(axis); }
    inline void loadRange() override { axis->setRange(bound.first, bound.second); }
    ~ValueAxisManager() { delete axis; }
};

#endif // AXISMANAGER_H
