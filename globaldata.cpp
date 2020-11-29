#include "globaldata.h"

GlobalData GlobalData::globalData;

GlobalData::GlobalData()
{

}

void GlobalData::env_set(QString key, QVariant value)
{
    environment[key] = value;
}

QVariant GlobalData::env_get(QString key) const
{
    return environment[key];
}

void GlobalData::conf_set(QString key, QVariant value)
{
    config[key] = value;
}

QVariant GlobalData::conf_get(QString key) const
{
    return config[key];
}
