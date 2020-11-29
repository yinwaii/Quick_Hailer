#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QMap>
#include <QObject>
#include <QVariant>

class GlobalData : public QObject
{
    Q_OBJECT
private:
    QMap<QString, QVariant> config;
    QMap<QString, QVariant> environment;

public:
    static GlobalData globalData;
    GlobalData();
    void conf_set(QString key, QVariant value);
    QVariant conf_get(QString key) const;
    void env_set(QString key, QVariant value);
    QVariant env_get(QString key) const;
};
#endif // GLOBALDATA_H
