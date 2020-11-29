#ifndef DATABASE_H
#define DATABASE_H

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextStream>

class DataBase : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase db;

public:
    static DataBase dataBase;
    explicit DataBase(QObject *parent = nullptr);
    void init();
    void load();

signals:
    void statusText(QString status);
    void statusProgress(int progress);
};

#endif // DATABASE_H
