#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <QObject>

class MapManager : public QObject
{
    Q_OBJECT
public:
    explicit MapManager(QObject *parent = nullptr);

signals:

};

#endif // MAPMANAGER_H
