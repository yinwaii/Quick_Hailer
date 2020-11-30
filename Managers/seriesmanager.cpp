#include "seriesmanager.h"

SeriesManager::SeriesManager(QString idField, QStringList recodeFields, QObject *parent)
    : QObject(parent), fields(qMakePair(idField, recodeFields))
{
    foreach (QString recodeField, recodeFields) {
        serieses[recodeField] = new QSplineSeries;
    }
}

void SeriesManager::setType(QMap<QString, SeriesType> typeConfig)
{
    foreach (QString field, typeConfig.keys()) {
        assert(fields.first == field || fields.second.contains(field));
        QXYSeries *tmp;
        switch (typeConfig[field]) {
        case SplineSeries:
            tmp = new QSplineSeries;
            break;
        case LineSeries:
            tmp = new QLineSeries;
            break;
        case ScatterSeries:
            tmp = new QScatterSeries;
            break;
        default:
            assert(0);
        }
        tmp->replace(serieses[field]->points());
        delete serieses[field];
        serieses[field] = tmp;
    }
}

void SeriesManager::addCache(double id, QList<double> recodes)
{
    assert(fields.second.size() == recodes.size());
    //    assert(cache.isEmpty());
    //    assert(min_max_cache.isEmpty());
    for (int i = 0; i < recodes.size(); i++) {
        cache[fields.second[i]].append(QPointF(id, recodes[i]));
        if (!min_max_cache.contains(fields.second[i]))
            min_max_cache[fields.second[i]] = qMakePair(recodes[i], recodes[i]);
        else {
            if (recodes[i] < min_max_cache[fields.second[i]].first)
                min_max_cache[fields.second[i]].first = recodes[i];
            if (recodes[i] > min_max[fields.second[i]].second)
                min_max_cache[fields.second[i]].second = recodes[i];
        }
    }
}

void SeriesManager::loadRecodes()
{
    foreach (QString field, cache.keys()) {
        serieses[field]->replace(cache[field]);
        cache[field].clear();
    }
    min_max = min_max_cache;
    cache.clear();
    min_max_cache.clear();
}

void SeriesManager::clearRecode(QString field)
{
    min_max.remove(field);
    delete serieses[field];
    serieses.remove(field);
    fields.second.removeOne(field);
}

void SeriesManager::clearAllRecode()
{
    foreach (QString field, fields.second)
        clearRecode(field);
    fields.first.clear();
}

void SeriesManager::reset(QString idField, QStringList recodeFields)
{
    fields = qMakePair(idField, recodeFields);
    foreach (QString recodeField, recodeFields) {
        serieses[recodeField] = new QSplineSeries;
    }
}

SeriesManager::~SeriesManager()
{
    foreach (QString recodeField, fields.second) {
        delete serieses[recodeField];
    }
}
