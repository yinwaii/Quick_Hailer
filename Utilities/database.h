#ifndef DATABASE_H
#define DATABASE_H

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QGeoCoordinate>
#include <QMessageBox>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTextStream>

class DataBase : public QObject
{
    Q_OBJECT
private:
    //    QSqlDatabase db;

public:
    static QMutex db_mutex;
    static DataBase dataBase;
    explicit DataBase(QObject *parent = nullptr);
    QSqlDatabase get(QString name = "");
    void remove();
    void init();
    void load();
    void loadGrids();
    QVariantList getGrid();
    QVariantList getEntryExit(double start, double end, int step);
    QVariantList getRoute(double time, int step);
    QVariantList getRelateTime(QGeoCoordinate origin, QGeoCoordinate destination,
                               double delta = 0.005);
    QVariantList getRelateSpace(QGeoCoordinate origin, double time, double delta = 0.005);
    int searchNum(QString command);
    QVariantList searchDemand(int start, int end, int step, QList<int> grid);
    QVariantList searchDistribution(int start, int end, int time_max, double fee_max, int step);
    QVariantList searchRevenue(int start, int end, int step);
    QList<QVariant> search(QString command);
    QVariant searchTarget(QString command);
    ~DataBase();

signals:
    void statusText(QString status);
    void statusProgress(int progress);
};

#endif // DATABASE_H
