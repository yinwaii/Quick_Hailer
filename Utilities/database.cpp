#include "database.h"
#include "globaldata.h"

DataBase DataBase::dataBase;
DataBase::DataBase(QObject *parent) : QObject(parent) {}
void DataBase::init()
{
    emit statusText("Creating the database ...");
    db = QSqlDatabase::addDatabase("QSQLITE", "ride_hailing_data");
    db.setDatabaseName(QApplication::applicationFilePath() + ".db");
    emit statusText("Opening the database ...");
    if (!db.open()) {
        qDebug() << "Can't create the database! ";
        assert(0);
    }
    QSqlQuery query(db);
    emit statusText("Clearing the previous table ...");
    if (!query.exec("DROP TABLE IF EXISTS dataset")) {
        qDebug() << "fail to clear the previous table";
        qDebug() << query.lastError().text();
        assert(0);
    }
    if (!query.exec("DROP TABLE IF EXISTS grid")) {
        qDebug() << "fail to clear the previous table";
        qDebug() << query.lastError().text();
        assert(0);
    }
    emit statusText("Creating the dataset table ...");
    if (!query.exec("CREATE TABLE dataset (order_id TEXT, departure_time INT, end_time INT, "
                    "orig_lng FLOAT, "
                    "orig_lat FLOAT, dest_lng FLOAT, dest_lat FLOAT, fee FLOAT, time INT)")) {
        qDebug() << "fail to create the table";
        qDebug() << query.lastError().text();
        assert(0);
    }
    if (!query.exec("CREATE TABLE grid (grid_id INT, vertex0_lat FLOAT, vertex0_lng FLOAT, "
                    "vertex1_lat FLOAT, vertex1_lng FLOAT, vertex2_lat FLOAT, vertex2_lng FLOAT, "
                    "vertex3_lat FLOAT, vertex3_lng FLOAT)")) {
        qDebug() << "fail to create the table";
        qDebug() << query.lastError().text();
        assert(0);
    }
    emit statusText("Creating the index ...");
    if (!query.exec("CREATE INDEX timeIndex ON dataset (departure_time, end_time)")) {
        qDebug() << "fail to create the time index";
        qDebug() << query.lastError().text();
        assert(0);
    }
    //    if (!query.exec("CREATE INDEX originIndex ON dataset (orig_lng, orig_lat)")) {
    //        QMessageBox::warning(nullptr, "error", "fail to create the origin index");
    //        qDebug() << query.lastError().text();
    //        return;
    //    }
    //    if (!query.exec("CREATE INDEX destinationIndex ON dataset (dest_lng, dest_lat)")) {
    //        QMessageBox::warning(nullptr, "error", "fail to create the destination index");
    //        qDebug() << query.lastError().text();
    //        return;
    //    }
    if (!query.exec("CREATE INDEX timeNeededIndex ON dataset (time)")) {
        qDebug() << "fail to create the time-needed index";
        qDebug() << query.lastError().text();
        assert(0);
    }
    if (!query.exec("CREATE INDEX feeIndex ON dataset (fee)")) {
        qDebug() << "fail to create the fee index";
        qDebug() << query.lastError().text();
        assert(0);
    }
    db.close();
    emit statusText("Successfully init the database!");
}
void DataBase::load()
{
    emit statusText("Opening the database ...");
    if (!db.open()) {
        qDebug() << "Can't create the database! ";
        assert(0);
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
            qDebug() << "Can't load the csv to the database: " << dataSet;
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
                qDebug() << "Required field " + field + " not found!";
                assert(0);
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
        insertTable += ", time";
        insertValue += ", :time";
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
            foreach (QString field, dataIndex) {
                if (field != "order_id")
                    query.bindValue(":" + field, dataLine[dataIndexMap[field]]);
                else
                    query.bindValue(":" + field, "\'" + dataLine[dataIndexMap[field]] + "\'");
                //                qDebug() << field << dataLine[dataIndexMap[field]];
            }
            query.bindValue(":time",
                            QString("%1").arg(dataLine[dataIndexMap["end_time"]].toInt()
                                              - dataLine[dataIndexMap["departure_time"]].toInt()));
            if (!query.exec()) {
                qDebug() << "can't load the data\n" + insertSql;
                qDebug() << query.lastError().text();
                assert(0);
            }
        }
        db.commit();
        dataFile.close();
    }
    //    emit statusProgress(0);
    db.close();
    //    QMessageBox::information(nullptr, "success", "load already success!");
}

void DataBase::loadGrids()
{
    emit statusText("Opening the database ...");
    if (!db.open()) {
        qDebug() << "Can't create the database! ";
        assert(0);
    }
    QSqlQuery query(db);
    emit statusText("Filtering the grid ...");
    QFile dataFile(GlobalData::globalData.conf_get("DataPath").toString() + '/'
                   + "rectangle_grid_table.csv");
    //        qDebug() << GlobalData::globalData.conf_get("DataPath").toString() + '/' + dataSet;
    if (!dataFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Can't load the csv to the grid";
        assert(0);
    }
    emit statusText("Mapping the data ...");
    QTextStream dataStream(&dataFile);
    QStringList dataIndex = dataStream.readLine().simplified().split(',');
    QMap<QString, int> dataIndexMap;
    foreach (QString field, GlobalData::globalData.conf_get("Grids").toStringList()) {
        dataIndexMap[field] = dataIndex.indexOf(field);
        if (dataIndexMap[field] == -1) {
            qDebug() << "Required field " + field + " not found!";
            assert(0);
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
    QString insertSql = "INSERT INTO grid (" + insertTable + ") VALUES (" + insertValue + ")";
    query.prepare(insertSql);
    //        qDebug() << "Preparing the instruction: " + insertSql + " ...";
    emit statusText("Loading the grid ...");
    while (!dataStream.atEnd()) {
        QStringList dataLine = dataStream.readLine().simplified().split(',');
        //            emit statusProgress(dataStream.pos() * 100 / dataFile.size());
        //            qDebug() << dataStream.pos() << dataFile.size();
        foreach (QString field, dataIndex) {
            query.bindValue(":" + field, dataLine[dataIndexMap[field]]);
            //                qDebug() << field << dataLine[dataIndexMap[field]];
        }
        if (!query.exec()) {
            qDebug() << "can't load the grid" << endl << insertSql;
            qDebug() << query.lastError().text();
            assert(0);
        }
    }
    db.commit();
    dataFile.close();
}

int DataBase::searchNum(QString command)
{
    emit statusText("Opening the database to get the num...");
    if (!db.open()) {
        QMessageBox::warning(nullptr, "warning", "Can't create the database! ");
        return -1;
    }
    qDebug() << command;
    QSqlQuery query(command, db);
    query.exec();
    int result = 0;
    while (query.next()) {
        result++;
    }
    //    qDebug() << "result:" << result;
    db.close();
    return result;
}

QList<QVariant> DataBase::search(QString command)
{
    emit statusText("Opening the database to search...");
    if (!db.open()) {
        QMessageBox::warning(nullptr, "warning", "Can't create the database! ");
        assert(0);
    }
    qDebug() << command;
    QSqlQuery query(command, db);
    QList<QVariant> list;
    query.exec();
    while (query.next()) {
        list.push_back(query.value(query.record().indexOf("Target")));
    }
    db.close();
    //    qDebug() << list;
    return list;
}

QVariant DataBase::searchTarget(QString command)
{
    emit statusText("Opening the database to get the target...");
    if (!db.open()) {
        QMessageBox::warning(nullptr, "warning", "Can't create the database! ");
        assert(0);
    }
    qDebug() << command;
    QSqlQuery query(command, db);
    QVariant result;
    query.exec();
    assert(query.next());
    result = query.value(query.record().indexOf("Target"));
    db.close();
    //    qDebug() << list;
    return result;
}

QVariantList DataBase::getGrid()
{
    emit statusText("Opening the database to get grids...");
    if (!db.open()) {
        QMessageBox::warning(nullptr, "warning", "Can't create the database! ");
        assert(0);
    }
    QVariantList result;
    QSqlQuery query("SELECT * FROM grid", db);
    while (query.next()) {
        QVariantMap tmp;
        QPointF topLeft = {query.value(query.record().indexOf("vertex0_lat")).toDouble(),
                           query.value(query.record().indexOf("vertex0_lng")).toDouble()};
        QPointF bottomRight = {query.value(query.record().indexOf("vertex2_lat")).toDouble(),
                               query.value(query.record().indexOf("vertex2_lng")).toDouble()};
        QRectF rect;
        rect.setTopLeft(topLeft);
        rect.setBottomRight(bottomRight);
        tmp["grid"] = rect;
        tmp["entry"] = -1;
        tmp["exit"] = -1;
        result.push_back(tmp);
    }
    db.close();
    return result;
}

QVariantList DataBase::getEntryExit(double start, double end, int step)
{
    int maxEntry = -1, maxExit = -1;
    emit statusText("Opening the database to get entry & exit...");
    if (!db.open()) {
        QMessageBox::warning(nullptr, "warning", "Can't create the database! ");
        assert(0);
    }
    QVariantList result;
    for (int i = 0; i < step * step; i++) {
        QVariantMap tmp;
        QPointF topLeft = {GlobalData::globalData.get_edge_lat(step, i / step + 1),
                           GlobalData::globalData.get_edge_lng(step, i % step)};
        QPointF bottomRight = {GlobalData::globalData.get_edge_lat(step, i / step),
                               GlobalData::get_edge_lng(step, i % step + 1)};
        QRectF rect;
        rect.setTopLeft(topLeft);
        rect.setBottomRight(bottomRight);
        tmp["grid"] = rect;
        tmp["entry"] = 0;
        tmp["exit"] = 0;
        result.push_back(tmp);
    }
    QString commandEntry
        = QString("SELECT * FROM dataset WHERE departure_time > %1 AND departure_time < %2")
              .arg(start)
              .arg(end);
    QSqlQuery query(commandEntry, db);
    while (query.next()) {
        int entryIndex = GlobalData::globalData
                             .get_grid(step,
                                       query.value(query.record().indexOf("orig_lng")).toDouble(),
                                       query.value(query.record().indexOf("orig_lat")).toDouble());
        //        qDebug() << query.value(query.record().indexOf("orig_lng")).toDouble()
        //                 << query.value(query.record().indexOf("orig_lat")).toDouble() << entryIndex;
        if (entryIndex >= step * step || entryIndex < 0)
            continue;
        QVariantMap entryMap = result[entryIndex].toMap();
        entryMap["entry"] = entryMap["entry"].toInt() + 1;
        result[entryIndex] = entryMap;
        if (maxEntry == -1 || entryMap["entry"].toInt() > maxEntry)
            maxEntry = entryMap["entry"].toInt();
    }
    QString commandExit
        = QString("SELECT * FROM dataset WHERE end_time > %1 AND end_time < %2").arg(start).arg(end);
    query.exec(commandExit);
    while (query.next()) {
        int exitIndex = GlobalData::globalData
                            .get_grid(step,
                                      query.value(query.record().indexOf("dest_lng")).toDouble(),
                                      query.value(query.record().indexOf("dest_lat")).toDouble());
        //        qDebug() << query.value(query.record().indexOf("dest_lng")).toDouble()
        //                 << query.value(query.record().indexOf("dest_lat")).toDouble() << exitIndex;
        if (exitIndex >= step * step || exitIndex < 0)
            continue;
        QVariantMap exitMap = result[exitIndex].toMap();
        exitMap["exit"] = exitMap["exit"].toInt() + 1;
        result[exitIndex] = exitMap;
        if (maxExit == -1 || exitMap["exit"].toInt() > maxExit)
            maxExit = exitMap["exit"].toInt();
    }
    for (int i = 0; i < result.count(); i++) {
        QVariantMap tmp = result[i].toMap();
        tmp["entry"] = double(result[i].toMap()["entry"].toInt()) / double(maxEntry);
        tmp["exit"] = double(result[i].toMap()["exit"].toInt()) / double(maxExit);
        result[i] = tmp;
    }
    db.close();
    return result;
}

QVariantList DataBase::getRoute(double time, int step)
{
    emit statusText("Opening the database to get route...");
    if (!db.open()) {
        QMessageBox::warning(nullptr, "warning", "Can't create the database! ");
        assert(0);
    }
    QVariantList result;
    QString commandRoute = QString(
                               "SELECT * FROM dataset WHERE departure_time < %1 AND end_time > %2")
                               .arg(time)
                               .arg(time);
    QSqlQuery query(commandRoute, db);
    QMap<QPair<int, int>, int> routeMap;
    while (query.next()) {
        QGeoCoordinate origin = {query.value(query.record().indexOf("orig_lat")).toDouble(),
                                 query.value(query.record().indexOf("orig_lng")).toDouble()};
        QGeoCoordinate destination = {query.value(query.record().indexOf("dest_lat")).toDouble(),
                                      query.value(query.record().indexOf("dest_lng")).toDouble()};
        QPair<int, int> tmpRouteGrid = qMakePair(
            GlobalData::globalData.get_grid(step, origin.longitude(), origin.latitude()),
            GlobalData::globalData.get_grid(step, destination.longitude(), destination.latitude()));
        if (routeMap.contains(tmpRouteGrid))
            routeMap[tmpRouteGrid]++;
        else
            routeMap[tmpRouteGrid] = 0;
        //        tmp["origin"] = QVariant::fromValue(origin);
        //        tmp["destination"] = QVariant::fromValue(destination);
        //        result.push_back(tmp);
    }
    int routeMax = 0;
    foreach (int routeCount, routeMap.values()) {
        if (routeCount > routeMax)
            routeMax = routeCount;
    }
    foreach (auto routePoint, routeMap.keys()) {
        QVariantMap tmp;
        tmp["origin"] = QVariant::fromValue(
            GlobalData::globalData.get_coordinate(step, routePoint.first));
        tmp["destination"] = QVariant::fromValue(
            GlobalData::globalData.get_coordinate(step, routePoint.second));
        tmp["count"] = double(routeMap[routePoint]) / double(routeMax);
        if (tmp["count"] > 0.4)
            result.push_back(tmp);
    }
    db.close();
    return result;
}

DataBase::~DataBase()
{
    //    db.removeDatabase("ride_hailing_data");
}
