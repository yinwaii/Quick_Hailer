#include "axismanager.h"

void AxisManager::loadConfig()
{
    foreach (AxisConfigItem conf_item, config.keys()) {
        switch (conf_item) {
        case TickCount:
            setTickCount(config[conf_item].toInt());
            break;
        case Format:
            setFormat(config[conf_item].toString());
            break;
        case LabelsAngle:
            setLabelsAngle(config[conf_item].toInt());
            break;
        case Range:
            setRange(config[conf_item].toPointF().x(), config[conf_item].toPointF().y());
            break;
        default:
            assert(0);
        }
    }
}

void AxisManager::init()
{
    AxisManager::internal_date.clear();
    AxisManager::internal_date[TickCount] = 12;
    AxisManager::internal_date[Format] = QString("MM-dd HH:mm");
    AxisManager::internal_date[LabelsAngle] = -90;
    AxisManager::internal_value.clear();
    AxisManager::internal_value[Format] = QString("%i");
}

QMap<AxisManager::AxisConfigItem, QVariant> AxisManager::internal_date;
QMap<AxisManager::AxisConfigItem, QVariant> AxisManager::internal_value;
