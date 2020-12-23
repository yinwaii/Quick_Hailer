#include "database.h"
#include "globaldata.h"

QMutex DataBase::db_mutex;
DataBase DataBase::dataBase;
DataBase::DataBase(QObject *parent) : QObject(parent) {}
void DataBase::init()
{
    emit statusText("Creating the database ...");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "ride_hailing_data");
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
    //    if (!query.exec("CREATE INDEX timeNeededIndex ON dataset (time)")) {
    //        qDebug() << "fail to create the time-needed index";
    //        qDebug() << query.lastError().text();
    //        assert(0);
    //    }
    //    if (!query.exec("CREATE INDEX feeIndex ON dataset (fee)")) {
    //        qDebug() << "fail to create the fee index";
    //        qDebug() << query.lastError().text();
    //        assert(0);
    //    }
    db.close();
    emit statusText("Successfully init the database!");
}
QSqlDatabase DataBase::get(QString name)
{
    QString connectionName = "ride_hailing_data";
    if (!name.isEmpty())
        connectionName = connectionName + "_" + name;
    connectionName += QDateTime::currentDateTime().toString("_hh_mm_ss");
    connectionName += QString("%1").arg(qrand());
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(QApplication::applicationFilePath() + ".db");
    db_mutex.lock();
    return db;
}
void DataBase::remove()
{
    db_mutex.unlock();
    QSqlDatabase::removeDatabase(QApplication::applicationFilePath() + ".db");
}
void DataBase::load()
{
    QSqlDatabase db = get();
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
    remove();
    //    QMessageBox::information(nullptr, "success", "load already success!");
}

void DataBase::loadGrids()
{
    QSqlDatabase db = get("loadGrids");
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
    db.close();
    remove();
}

int DataBase::searchNum(QString command)
{
    QSqlDatabase db = get("num");
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
    remove();
    return result;
}

QVariantList DataBase::searchDemand(int start, int end, int step, QList<int> grid)
{
    QSqlDatabase db = get("demand");
    emit statusText("Opening the database to get the num...");
    if (!db.open()) {
        QMessageBox::warning(nullptr, "warning", "Can't create the database! ");
        assert(0);
    }
    QString command = QString("SELECT * FROM dataset WHERE departure_time>%1 AND end_time<%2")
                          .arg(start)
                          .arg(end);
    QSqlQuery query(command, db);
    query.exec();
    QVariantList result;
    for (int i = 0; i < step; i++) {
        QVariantMap stepMap;
        stepMap["entry"] = 0;
        stepMap["exit"] = 0;
        result.push_back(stepMap);
    }
    int querySize = 0;
    if (query.last()) {
        querySize = query.at() + 1;
        query.first();
        query.previous();
    }
    int progress = 0;
    while (query.next()) {
        int new_progress = floor(double(query.at() * 100) / double(querySize));
        if (new_progress > progress) {
            progress = new_progress;
            GlobalData::globalData.statusText(
                QString("Demand Chart: filtering the data %1\% ...").arg(progress));
            GlobalData::globalData.statusProgress(progress);
        }
        //        qDebug() << query.at() << querySize;
        int gridIndex
            = GlobalData::get_grid(10,
                                   query.value(query.record().indexOf("orig_lng")).toDouble(),
                                   query.value(query.record().indexOf("orig_lat")).toDouble());
        if (!grid.contains(gridIndex))
            continue;
        QVariantMap old_entry_map
            = result[GlobalData::get_step(step,
                                          query.value(query.record().indexOf("departure_time"))
                                              .toDouble(),
                                          start,
                                          end)]
                  .toMap();
        int old_entry = old_entry_map["entry"].toInt();
        old_entry_map["entry"] = old_entry + 1;
        result[GlobalData::get_step(
            step, query.value(query.record().indexOf("departure_time")).toDouble(), start, end)]
            = old_entry_map;
        QVariantMap old_exit_map
            = result[GlobalData::get_step(step,
                                          query.value(query.record().indexOf("end_time")).toDouble(),
                                          start,
                                          end)]
                  .toMap();
        int old_exit = old_exit_map["exit"].toInt();
        old_exit_map["exit"] = old_exit + 1;
        result[GlobalData::get_step(
            step, query.value(query.record().indexOf("end_time")).toDouble(), start, end)]
            = old_exit_map;
    }
    //    qDebug() << "result:" << result;
    GlobalData::globalData.statusProgress(0);
    db.close();
    remove();
    return result;
}

QVariantList DataBase::searchDistribution(int start, int end, int time_max, double fee_max, int step)
{
    QSqlDatabase db = get("distribution");
    emit statusText("Opening the database to get the num...");
    if (!db.open()) {
        QMessageBox::warning(nullptr, "warning", "Can't create the database! ");
        assert(0);
    }
    QString command = QString("SELECT * FROM dataset WHERE departure_time>%1 AND departure_time<%2")
                          .arg(start)
                          .arg(end);
    QSqlQuery query(command, db);
    query.exec();
    QVariantList result;
    for (int i = 0; i < step; i++) {
        QVariantMap stepMap;
        stepMap["time"] = 0;
        stepMap["fee"] = 0;
        result.push_back(stepMap);
    }
    int querySize = 0;
    if (query.last()) {
        querySize = query.at() + 1;
        query.first();
        query.previous();
    }
    int progress = 0;
    while (query.next()) {
        int new_progress = floor(double(query.at() * 100) / double(querySize));
        if (new_progress > progress) {
            progress = new_progress;
            GlobalData::globalData.statusText(
                QString("Distribution Chart: filtering the data %1\% ...").arg(progress));
            GlobalData::globalData.statusProgress(progress);
        }
        if (GlobalData::get_step(step,
                                 query.value(query.record().indexOf("time")).toDouble(),
                                 0,
                                 time_max)
            < step) {
            //            qDebug() << GlobalData::get_step(step,
            //                                             query.value(query.record().indexOf("time")).toDouble(),
            //                                             0,
            //                                             time_max);
            QVariantMap old_time_map
                = result[GlobalData::get_step(step,
                                              query.value(query.record().indexOf("time")).toDouble(),
                                              0,
                                              time_max)]
                      .toMap();
            int old_entry = old_time_map["time"].toInt();
            old_time_map["time"] = old_entry + 1;
            result[GlobalData::get_step(
                step, query.value(query.record().indexOf("time")).toDouble(), 0, time_max)]
                = old_time_map;
        }
        if (GlobalData::get_step(step,
                                 query.value(query.record().indexOf("fee")).toDouble(),
                                 0,
                                 fee_max)
            < step) {
            //            qDebug() << GlobalData::get_step(step,
            //                                             query.value(query.record().indexOf("fee")).toDouble(),
            //                                             0,
            //                                             fee_max);
            QVariantMap old_fee_map
                = result[GlobalData::get_step(
                             step, query.value(query.record().indexOf("fee")).toDouble(), 0, fee_max)]
                      .toMap();
            int old_fee = old_fee_map["fee"].toInt();
            old_fee_map["fee"] = old_fee + 1;
            result[GlobalData::get_step(
                step, query.value(query.record().indexOf("fee")).toDouble(), 0, fee_max)]
                = old_fee_map;
        }
    }
    GlobalData::globalData.statusProgress(0);
    //    qDebug() << "result:" << result;
    db.close();
    remove();
    return result;
}

QVariantList DataBase::searchRevenue(int start, int end, int step)
{
    QSqlDatabase db = get("revenue");
    emit statusText("Opening the database to get the num...");
    if (!db.open()) {
        QMessageBox::warning(nullptr, "warning", "Can't create the database! ");
        assert(0);
    }
    QString command = QString("SELECT * FROM dataset WHERE departure_time>%1 AND end_time<%2")
                          .arg(start)
                          .arg(end);
    QSqlQuery query(command, db);
    query.exec();
    QVariantList result;
    for (int i = 0; i < step; i++) {
        QVariantMap stepMap;
        stepMap["fee"] = 0;
        result.push_back(stepMap);
    }
    int querySize = 0;
    if (query.last()) {
        querySize = query.at() + 1;
        query.first();
        query.previous();
    }
    int progress = 0;
    while (query.next()) {
        int new_progress = floor(double(query.at() * 100) / double(querySize));
        if (new_progress > progress) {
            progress = new_progress;
            GlobalData::globalData.statusText(
                QString("Revenue Chart: filtering the data %1\% ...").arg(progress));
            GlobalData::globalData.statusProgress(progress);
        }
        QVariantMap old_fee_map
            = result[GlobalData::get_step(step,
                                          query.value(query.record().indexOf("end_time")).toDouble(),
                                          start,
                                          end)]
                  .toMap();
        double old_fee = old_fee_map["fee"].toDouble();
        old_fee_map["fee"] = old_fee + query.value(query.record().indexOf("fee")).toDouble();
        result[GlobalData::get_step(
            step, query.value(query.record().indexOf("end_time")).toDouble(), start, end)]
            = old_fee_map;
    }
    GlobalData::globalData.statusProgress(0);
    //    qDebug() << "result:" << result;
    db.close();
    remove();
    return result;
}

QList<QVariant> DataBase::search(QString command)
{
    QSqlDatabase db = get("search");
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
    remove();
    //    qDebug() << list;
    return list;
}

QVariant DataBase::searchTarget(QString command)
{
    QSqlDatabase db = get("target");
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
    remove();
    //    qDebug() << list;
    return result;
}

QVariantList DataBase::getGrid()
{
    QSqlDatabase db = get("getGrid");
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
        tmp["selected"] = false;
        result.push_back(tmp);
    }
    db.close();
    remove();
    return result;
}

QVariantList DataBase::getEntryExit(double start, double end, int step)
{
    QSqlDatabase db = get("entryExit");
    //    int maxEntry = -1, maxExit = -1;
    //    emit statusText("Opening the database to get entry & exit...");
    GlobalData::globalData.statusText("Thermal Map: Opening the database to get entry & exit...");
    if (!db.open()) {
        QMessageBox::warning(nullptr, "warning", "Can't create the database! ");
        assert(0);
    }
    QVariantList result;
    QMap<int, QPair<int, int>> entryExitMap;
    //    for (int i = 0; i < step * step; i++) {
    //        QVariantMap tmp;
    //        QPointF topLeft = {GlobalData::globalData.get_edge_lat(step, i / step + 1),
    //                           GlobalData::globalData.get_edge_lng(step, i % step)};
    //        QPointF bottomRight = {GlobalData::globalData.get_edge_lat(step, i / step),
    //                               GlobalData::get_edge_lng(step, i % step + 1)};
    //        QRectF rect;
    //        rect.setTopLeft(topLeft);
    //        rect.setBottomRight(bottomRight);
    //        tmp["grid"] = rect;
    //        tmp["entry"] = 0;
    //        tmp["exit"] = 0;
    //        result.push_back(tmp);
    //    }
    QString commandEntry
        = QString("SELECT * FROM dataset WHERE departure_time >= %1 AND departure_time <= %2")
              .arg(int(start))
              .arg(int(end));
    QSqlQuery query(commandEntry, db);
    int querySizeEntry = 0;
    if (query.last()) {
        querySizeEntry = query.at() + 1;
        query.first();
        query.previous();
    }
    int progressEntry = 0;
    while (query.next()) {
        int new_progress = floor(double(query.at() * 100) / double(querySizeEntry));
        if (new_progress > progressEntry) {
            progressEntry = new_progress;
            GlobalData::globalData.statusText(
                QString("Thermal Map: filtering the entry data %1\% ...").arg(progressEntry));
            GlobalData::globalData.statusProgress(progressEntry);
        }
        //    qDebug() << commandEntry;
        int entryIndex = GlobalData::globalData
                             .get_grid(step,
                                       query.value(query.record().indexOf("orig_lng")).toDouble(),
                                       query.value(query.record().indexOf("orig_lat")).toDouble());
        if (entryExitMap.contains(entryIndex))
            entryExitMap[entryIndex].first++;
        else
            entryExitMap[entryIndex] = qMakePair(1, 0);
        //        qDebug() << query.value(query.record().indexOf("orig_lng")).toDouble()
        //                 << query.value(query.record().indexOf("orig_lat")).toDouble() << entryIndex;
        if (entryIndex >= step * step || entryIndex < 0)
            continue;
        //        QVariantMap entryMap = result[entryIndex].toMap();
        //        entryMap["entry"] = entryMap["entry"].toInt() + 1;
        //        result[entryIndex] = entryMap;
        //        if (maxEntry == -1 || entryMap["entry"].toInt() > maxEntry)
        //            maxEntry = entryMap["entry"].toInt();
    }
    GlobalData::globalData.statusProgress(0);
    QString commandExit = QString("SELECT * FROM dataset WHERE end_time >= %1 AND end_time <= %2")
                              .arg(int(start))
                              .arg(int(end));
    query.exec(commandExit);
    //    qDebug() << commandExit;
    int querySizeExit = 0;
    if (query.last()) {
        querySizeExit = query.at() + 1;
        query.first();
        query.previous();
    }
    int progressExit = 0;
    while (query.next()) {
        int new_progressExit = floor(double(query.at() * 100) / double(querySizeExit));
        if (new_progressExit > progressExit) {
            progressExit = new_progressExit;
            GlobalData::globalData.statusText(
                QString("Thermal Map: filtering the exit data %1\% ...").arg(progressExit));
            GlobalData::globalData.statusProgress(progressExit);
        }
        int exitIndex = GlobalData::globalData
                            .get_grid(step,
                                      query.value(query.record().indexOf("dest_lng")).toDouble(),
                                      query.value(query.record().indexOf("dest_lat")).toDouble());
        if (entryExitMap.contains(exitIndex))
            entryExitMap[exitIndex].second++;
        else
            entryExitMap[exitIndex] = qMakePair(0, 1);
        //        qDebug() << query.value(query.record().indexOf("dest_lng")).toDouble()
        //                 << query.value(query.record().indexOf("dest_lat")).toDouble() << exitIndex;
        if (exitIndex >= step * step || exitIndex < 0)
            continue;
        //        QVariantMap exitMap = result[exitIndex].toMap();
        //        exitMap["exit"] = exitMap["exit"].toInt() + 1;
        //        result[exitIndex] = exitMap;
        //        if (maxExit == -1 || exitMap["exit"].toInt() > maxExit)
        //            maxExit = exitMap["exit"].toInt();
    }
    GlobalData::globalData.statusProgress(0);
    //    qDebug() << entryExitMap;
    int entryMax = 0, exitMax = 0;
    foreach (int index, entryExitMap.keys()) {
        if (entryExitMap[index].first > entryMax)
            entryMax = entryExitMap[index].first;
        if (entryExitMap[index].second > exitMax)
            exitMax = entryExitMap[index].second;
    }
    GlobalData::globalData.statusText("Thermal Map: Pushing the data ...");
    foreach (int index, entryExitMap.keys()) {
        QVariantMap tmp;
        QPointF topLeft = {GlobalData::globalData.get_edge_lat(step, index / step + 1),
                           GlobalData::globalData.get_edge_lng(step, index % step)};
        QPointF bottomRight = {GlobalData::globalData.get_edge_lat(step, index / step),
                               GlobalData::get_edge_lng(step, index % step + 1)};
        QRectF rect;
        rect.setTopLeft(topLeft);
        rect.setBottomRight(bottomRight);
        tmp["grid"] = rect;
        tmp["entry"] = double(entryExitMap[index].first) / double(entryMax);
        tmp["exit"] = double(entryExitMap[index].second) / double(entryMax);
        //        qDebug() << tmp;
        if (tmp["entry"] > 0.1 || tmp["exit"] > 0.1)
            result.push_back(tmp);
    }
    //    for (int i = 0; i < result.count(); i++) {
    //        QVariantMap tmp = result[i].toMap();
    //        tmp["entry"] = double(result[i].toMap()["entry"].toInt()) / double(maxEntry);
    //        tmp["exit"] = double(result[i].toMap()["exit"].toInt()) / double(maxExit);
    //        result[i] = tmp;
    //    }
    db.close();
    remove();
    //    qDebug() << result;
    return result;
}

QVariantList DataBase::getRoute(double time, int step)
{
    QSqlDatabase db = get("route");
    //    emit statusText("Opening the database to get route...");
    GlobalData::globalData.statusText("Flow Map: Opening the database to get route ...");
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
    int querySize = 0;
    if (query.last()) {
        querySize = query.at() + 1;
        query.first();
        query.previous();
    }
    int progress = 0;
    while (query.next()) {
        int new_progress = floor(double(query.at() * 100) / double(querySize));
        if (new_progress > progress) {
            progress = new_progress;
            GlobalData::globalData.statusText(
                QString("Flow Map: filtering the data %1\% ...").arg(progress));
            GlobalData::globalData.statusProgress(progress);
        }
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
            routeMap[tmpRouteGrid] = 1;
        //        tmp["origin"] = QVariant::fromValue(origin);
        //        tmp["destination"] = QVariant::fromValue(destination);
        //        result.push_back(tmp);
    }
    GlobalData::globalData.statusProgress(0);
    int routeMax = 0;
    foreach (int routeCount, routeMap.values()) {
        if (routeCount > routeMax)
            routeMax = routeCount;
    }
    GlobalData::globalData.statusText("Flow Map: Pushing the data ...");
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
    remove();
    return result;
}

QVariantList DataBase::getRelateTime(QGeoCoordinate origin, QGeoCoordinate destination, double delta)
{
    QSqlDatabase db = get("relateTime");
    emit statusText("Opening the database to predict routes...");
    GlobalData::globalData.statusText("Time Prediction: Opening the database to predict time ...");
    if (!db.open()) {
        QMessageBox::warning(nullptr, "warning", "Can't create the database! ");
        assert(0);
    }
    QVariantList result;
    QString commandRoute = QString("SELECT * FROM dataset WHERE orig_lat >= %1 AND orig_lat <= %2 "
                                   "AND orig_lng >= %3 AND orig_lng <= %4 AND dest_lat >= %5 AND "
                                   "dest_lat <= %6 AND dest_lng >= %7 AND dest_lng <= %8")
                               .arg(origin.latitude() - delta)
                               .arg(origin.latitude() + delta)
                               .arg(origin.longitude() - delta)
                               .arg(origin.longitude() + delta)
                               .arg(destination.latitude() - delta)
                               .arg(destination.latitude() + delta)
                               .arg(destination.longitude() - delta)
                               .arg(destination.longitude() + delta);
    //    qDebug() << commandRoute;
    QSqlQuery query(commandRoute, db);
    int querySize = 0;
    if (query.last()) {
        querySize = query.at() + 1;
        query.first();
        query.previous();
    }
    int progress = 0;
    while (query.next()) {
        int new_progress = floor(double(query.at() * 100) / double(querySize));
        if (new_progress > progress) {
            progress = new_progress;
            GlobalData::globalData.statusText(
                QString("Time Prediction: filtering the data %1\% ...").arg(progress));
        }
        QVariantMap tmp;
        QGeoCoordinate origin = {query.value(query.record().indexOf("orig_lat")).toDouble(),
                                 query.value(query.record().indexOf("orig_lng")).toDouble()};
        QGeoCoordinate destination = {query.value(query.record().indexOf("dest_lat")).toDouble(),
                                      query.value(query.record().indexOf("dest_lng")).toDouble()};
        tmp["origin"] = QVariant::fromValue(origin);
        tmp["destination"] = QVariant::fromValue(destination);
        tmp["time"] = query.value(query.record().indexOf("time"));
        tmp["fee"] = query.value(query.record().indexOf("fee"));
        //        qDebug() << "get" << tmp;
        result.push_back(tmp);
    }
    db.close();
    remove();
    return result;
}

QVariantList DataBase::getRelateSpace(QGeoCoordinate origin, double time, double delta)
{
    QSqlDatabase db = get("relateSpace");
    emit statusText("Opening the database to predict space...");
    GlobalData::globalData.statusText(
        "Space Prediction: Opening the database to predict space ...");
    if (!db.open()) {
        QMessageBox::warning(nullptr, "warning", "Can't create the database! ");
        assert(0);
    }
    QVariantList result;
    qDebug() << origin << time;
    QString commandRoute = QString(
                               "SELECT * FROM dataset WHERE orig_lat >= %1 AND orig_lat <= %2 "
                               "AND orig_lng >= %3 AND orig_lng <= %4 AND time >= %5 AND time <=%6")
                               .arg(origin.latitude() - delta)
                               .arg(origin.latitude() + delta)
                               .arg(origin.longitude() - delta)
                               .arg(origin.longitude() + delta)
                               .arg(time * 60 - 20)
                               .arg(time * 60 + 20);
    qDebug() << commandRoute;
    QSqlQuery query(commandRoute, db);
    int querySize = 0;
    if (query.last()) {
        querySize = query.at() + 1;
        query.first();
        query.previous();
    }
    int progress = 0;
    while (query.next()) {
        int new_progress = floor(double(query.at() * 100) / double(querySize));
        if (new_progress > progress) {
            progress = new_progress;
            GlobalData::globalData.statusText(
                QString("Space Prediction: filtering the data %1\% ...").arg(progress));
        }
        QVariantMap tmp;
        QGeoCoordinate origin = {query.value(query.record().indexOf("orig_lat")).toDouble(),
                                 query.value(query.record().indexOf("orig_lng")).toDouble()};
        QGeoCoordinate destination = {query.value(query.record().indexOf("dest_lat")).toDouble(),
                                      query.value(query.record().indexOf("dest_lng")).toDouble()};
        tmp["origin"] = QVariant::fromValue(origin);
        tmp["destination"] = QVariant::fromValue(destination);
        tmp["time"] = query.value(query.record().indexOf("time"));
        tmp["fee"] = query.value(query.record().indexOf("fee"));
        result.push_back(tmp);
    }
    db.close();
    remove();
    return result;
}

DataBase::~DataBase()
{
    //    db.removeDatabase("ride_hailing_data");
}
