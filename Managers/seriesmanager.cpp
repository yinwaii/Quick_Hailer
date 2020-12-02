#include "seriesmanager.h"

SeriesManager::SeriesManager(QString idField, QStringList recodeFields, QObject *parent)
    : QObject(parent)
{
    fields[idField] = recodeFields;
    foreach (QString recodeField, recodeFields) {
        serieses[recodeField] = new QSplineSeries;
    }
}

void SeriesManager::addId(QString idField, QStringList recodeFields)
{
    //    qDebug() << serieses;
    fields[idField] = recodeFields;
    foreach (QString recodeField, recodeFields) {
        serieses[recodeField] = new QSplineSeries;
    }
    //    qDebug() << serieses;
}

void SeriesManager::setType(QMap<QString, SeriesType> typeConfig)
{
    foreach (QString field, typeConfig.keys()) {
        assert(serieses.keys().contains(field));
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

void SeriesManager::addCache(QString idField, double id, QList<double> recodes)
{
    assert(fields.keys().contains(idField));
    assert(fields[idField].size() == recodes.size());
    //    assert(cache.isEmpty());
    //    assert(min_max_cache.isEmpty());
    for (int i = 0; i < recodes.size(); i++) {
        cache[fields[idField][i]].append(QPointF(id, recodes[i]));
        if (!min_max_cache.contains(fields[idField][i]))
            min_max_cache[fields[idField][i]] = qMakePair(recodes[i], recodes[i]);
        else {
            if (recodes[i] < min_max_cache[fields[idField][i]].first)
                min_max_cache[fields[idField][i]].first = recodes[i];
            if (recodes[i] > min_max[fields[idField][i]].second)
                min_max_cache[fields[idField][i]].second = recodes[i];
        }
    }
}

void SeriesManager::loadRecodes()
{
    qDebug() << cache;
    qDebug() << serieses;
    foreach (QString field, cache.keys()) {
        serieses[field]->replace(cache[field]);
        serieses[field]->setName(field);
        cache[field].clear();
    }
    min_max = min_max_cache;
    cache.clear();
    min_max_cache.clear();
}

void SeriesManager::clearRecode(QString id, QString field)
{
    min_max.remove(field);
    delete serieses[field];
    serieses.remove(field);
    fields[id].removeOne(field);
}

void SeriesManager::clearAllRecode()
{
    foreach (QString id, fields.keys()) {
        foreach (QString field, fields[id])
            clearRecode(id, field);
    }
    fields.clear();
}

void SeriesManager::reset(QStringList idFields, QMap<QString, QStringList> recodeFields,
                          QMap<QString, SeriesManager::SeriesType> typeConfig)
{
    clearAllRecode();
    foreach (QString idField, idFields) {
        fields[idField] = recodeFields[idField];
        foreach (QString recodeField, recodeFields[idField]) {
            if (typeConfig.isEmpty())
                serieses[recodeField] = new QSplineSeries;
            else {
                switch (typeConfig[recodeField]) {
                case SplineSeries:
                    serieses[recodeField] = new QSplineSeries;
                    break;
                case LineSeries:
                    serieses[recodeField] = new QLineSeries;
                    break;
                case ScatterSeries:
                    serieses[recodeField] = new QScatterSeries;
                    break;
                default:
                    assert(0);
                }
            }
        }
    }
    loadRecodes();
}

SeriesManager::~SeriesManager()
{
    foreach (QString id, fields.keys()) {
        foreach (QString recodeField, fields[id]) {
            delete serieses[recodeField];
        }
    }
}
