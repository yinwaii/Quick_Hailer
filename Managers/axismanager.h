#ifndef AXISMANAGER_H
#define AXISMANAGER_H

#include <QObject>
#include <QtCharts>

class AxisManager : public QObject
{
    Q_OBJECT

public:
    enum AxisConfigItem { TickCount, Format, LabelsAngle, Range };
    static QMap<AxisConfigItem, QVariant> internal_date;
    static QMap<AxisConfigItem, QVariant> internal_value;
    AxisManager(Qt::Alignment alignment) : align(alignment) { init(); }
    virtual void setTickCount(int num) = 0;
    virtual void setFormat(QString format) = 0;
    virtual void setLabelsAngle(int angle) = 0;
    virtual void addAxis(QChart *chart) = 0;
    virtual void attachSeries(QXYSeries *series) = 0;
    virtual void setRange(QPair<double, double> minmax) = 0;
    virtual void setRange(double min, double max) = 0;
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
    inline void setRange(QPair<double, double> minmax) override
    {
        axis->setRange(QDateTime::fromTime_t(int(minmax.first)),
                       QDateTime::fromTime_t(int(minmax.second)));
    }
    inline void setRange(double min, double max) override
    {
        axis->setRange(QDateTime::fromTime_t(int(min)), QDateTime::fromTime_t(int(max)));
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
    inline void setRange(QPair<double, double> minmax) override
    {
        axis->setRange(minmax.first, minmax.second);
    }
    inline void setRange(double min, double max) override { axis->setRange(min, max); }
    ~ValueAxisManager() { delete axis; }
};

#endif // AXISMANAGER_H
