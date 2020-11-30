#ifndef SERIESMANAGER_H
#define SERIESMANAGER_H

#include <QObject>

class SeriesManager : public QObject
{
    Q_OBJECT
public:
    explicit SeriesManager(QObject *parent = nullptr);

signals:

};

#endif // SERIESMANAGER_H
