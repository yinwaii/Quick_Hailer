#include "database.h"
#include "globaldata.h"

DataBase DataBase::dataBase;
DataBase::DataBase(QObject *parent) : QObject(parent) {}
void DataBase::init()
{
    emit statusText("Creating the database ...");
    db = QSqlDatabase::addDatabase("QSQLITE", "ride_hailing_data");
    db.setDatabaseName(QApplication::applicationFilePath() + "database.db");
    emit statusText("Opening the database ...");
    if (!db.open()) {
        QMessageBox::warning(nullptr, "warning", "Can't create the database! ");
        return;
    }
    QSqlQuery query(db);
    emit statusText("Clearing the previous table ...");
    if (!query.exec("DROP TABLE IF EXISTS dataset")) {
        QMessageBox::warning(nullptr, "error", "fail to clear the previous table");
        qDebug() << query.lastError().text();
        return;
    }
    emit statusText("Creating the dataset table ...");
    if (!query.exec("CREATE TABLE dataset (order_id TEXT, departure_time FLOAT, end_time FLOAT, "
                    "orig_lng FLOAT, "
                    "orig_lat FLOAT, dest_lng FLOAT, dest_lat FLOAT, fee FLOAT)")) {
        QMessageBox::warning(nullptr, "error", "fail to create the table");
        qDebug() << query.lastError().text();
        return;
    }
    emit statusText("Creating the index ...");
    if (!query.exec("CREATE INDEX timeIndex ON dataset (departure_time, end_time)")) {
        QMessageBox::warning(nullptr, "error", "fail to create the time index");
        qDebug() << query.lastError().text();
        return;
    }
    if (!query.exec("CREATE INDEX originIndex ON dataset (orig_lng, orig_lat)")) {
        QMessageBox::warning(nullptr, "error", "fail to create the origin index");
        qDebug() << query.lastError().text();
        return;
    }
    if (!query.exec("CREATE INDEX destinationIndex ON dataset (dest_lng, dest_lat)")) {
        QMessageBox::warning(nullptr, "error", "fail to create the destination index");
        qDebug() << query.lastError().text();
        return;
    }
    db.close();
    emit statusText("Successfully init the database!");
}
void DataBase::load()
{
    emit statusText("Opening the database ...");
    if (!db.open()) {
        QMessageBox::warning(nullptr, "warning", "Can't create the database! ");
        return;
    }
    QSqlQuery query(db);
    emit statusText("Filtering the dataset ...");
    QDir dataDirectory(GlobalData::globalData.conf_get("DataPath").toString());
    QStringList dataSets;
    //    qDebug() << "dataPath" << GlobalData::globalData.conf_get("DataPath").toString();
    //    qDebug() << "listDates" << GlobalData::globalData.conf_get("Dates").toStringList();
    foreach (QString date, GlobalData::globalData.conf_get("Dates").toStringList()) {
        dataSets.append(dataDirectory.entryList(QStringList({"order_" + date + "*.csv",
                                                             "order_" + date + "*.CSV"}),
                                                QDir::Files));
    }
    //    qDebug() << "dataSets: " << dataSets;
    int datacnt = 0;
    foreach (QString dataSet, dataSets) {
        datacnt++;
        emit statusProgress(int(datacnt * 100 / dataSets.size()));
        emit statusText("Opening the data: " + dataSet + " ...");
        QFile dataFile(GlobalData::globalData.conf_get("DataPath").toString() + '/' + dataSet);
        //        qDebug() << GlobalData::globalData.conf_get("DataPath").toString() + '/' + dataSet;
        if (!dataFile.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(nullptr,
                                 "warning",
                                 "Can't load the csv to the database: " + dataSet);
            continue;
        }
        emit statusText("Mapping the data: " + dataSet + " ...");
        QTextStream dataStream(&dataFile);
        QStringList dataIndex = dataStream.readLine().simplified().split(',');
        QMap<QString, int> dataIndexMap;
        //        qDebug() << GlobalData::globalData.conf_get("Fields").toStringList();
        foreach (QString field, GlobalData::globalData.conf_get("Fields").toStringList()) {
            dataIndexMap[field] = dataIndex.indexOf(field);
            if (dataIndexMap[field] == -1) {
                QMessageBox::warning(nullptr, "error", "Required field " + field + " not found!");
                return;
            }
        }
        //        qDebug() << dataIndexMap;
        QString insertTable, insertValue;
        foreach (QString field, dataIndexMap.keys()) {
            if (!insertTable.isEmpty())
                insertTable += ", ";
            if (!insertValue.isEmpty())
                insertValue += ", ";
            insertTable += field;
            insertValue += (":" + field);
        }
        db.transaction();
        QString insertSql = "INSERT INTO dataset (" + insertTable + ") VALUES (" + insertValue
                            + ")";
        query.prepare(insertSql);
        //        qDebug() << "Preparing the instruction: " + insertSql + " ...";
        emit statusText("Loading the data: " + dataSet + " ...");
        while (!dataStream.atEnd()) {
            QStringList dataLine = dataStream.readLine().simplified().split(',');
            //            emit statusProgress(dataStream.pos() * 100 / dataFile.size());
            //            qDebug() << dataStream.pos() << dataFile.size();
            foreach (QString field, dataLine) {
                if (field != "order_id")
                    query.bindValue(":" + field, dataLine[dataIndexMap[field]]);
                else
                    query.bindValue(":" + field, "\'" + dataLine[dataIndexMap[field]] + "\'");
            }
            if (!query.exec()) {
                QMessageBox::warning(nullptr, "error", "can't load the data\n" + insertSql);
                qDebug() << query.lastError().text();
                return;
            }
        }
        db.commit();
        dataFile.close();
    }
    emit statusProgress(0);
    db.close();
    //    QMessageBox::information(nullptr, "success", "load already success!");
}