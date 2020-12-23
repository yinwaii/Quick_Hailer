#include "loadwindow.h"
#include "ui_loadwindow.h"

LoadWindow::LoadWindow(QWidget *parent) : QWidget(parent), ui(new Ui::LoadWindow)
{
    ui->setupUi(this);
    setWindowTitle("Load");
    connect(&DataBase::dataBase, &DataBase::statusText, this, &LoadWindow::statusText);
    connect(&DataBase::dataBase, &DataBase::statusProgress, this, &LoadWindow::statusProgress);
    ui->labelStatus->setHidden(true);
    ui->progressBar->setHidden(true);
    ui->boxDataSet->addItem("/Users/yinwai/Downloads/Dataset-CS241-2020-2");
}

LoadWindow::~LoadWindow()
{
    delete ui;
}

void LoadWindow::on_buttonDataSetOpen_clicked()
{
    QString dataPath = QFileDialog::getExistingDirectory(this, "Select dataset folder");
    if (dataPath.isNull()) {
        QMessageBox::warning(this,
                             "Warning",
                             "Can't open the dataset folder, because the folder path is Null. ");
        return;
    }
    QDir dataDirectory(dataPath);
    if (!dataDirectory.exists("rectangle_grid_table.csv")) {
        QMessageBox::warning(this, "Warning", "\"rectangle_grid_table.csv\" not found! ");
        return;
    }
    QStringList dataList = dataDirectory.entryList(QStringList({"order_*.csv", "order_*.CSV"}),
                                                   QDir::Files);
    if (dataList.empty()) {
        QMessageBox::warning(this, "Warning", "Order data not found! ");
        return;
    }
    int foundIndex = ui->boxDataSet->findText(dataPath);
    if (foundIndex != -1)
        ui->boxDataSet->removeItem(foundIndex);
    ui->boxDataSet->insertItem(0, dataPath);
    ui->boxDataSet->setCurrentIndex(0);
}

void LoadWindow::on_boxDataSet_currentIndexChanged(const QString &arg1)
{
    // Check the directory
    QDir dataDirectory(arg1);
    if (!dataDirectory.exists("rectangle_grid_table.csv")) {
        QMessageBox::warning(this, "Warning", "\"rectangle_grid_table.csv\" not found! ");
        ui->boxDataSet->removeItem(ui->boxDataSet->currentIndex());
        return;
    }
    QStringList dataList = dataDirectory.entryList(QStringList({"order_*.csv", "order_*.CSV"}),
                                                   QDir::Files);
    if (dataList.empty()) {
        QMessageBox::warning(this, "Warning", "Order data not found! ");
        ui->boxDataSet->removeItem(ui->boxDataSet->currentIndex());
        return;
    }
    QFile dataSample(dataDirectory.absoluteFilePath(dataList[0]));
    if (!dataSample.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Warning", "Can't read the csv data! ");
        ui->boxDataSet->removeItem(ui->boxDataSet->currentIndex());
        return;
    }
    // Show the Fields
    QTextStream dataStream(&dataSample);
    QStringList dataFields = dataStream.readLine().simplified().split(",");
    dataSample.close();
    foreach (QString field, checkFields.keys()) {
        layoutFields.removeWidget(checkFields[field]);
        delete checkFields[field];
    }
    checkFields.clear();
    foreach (QString field, dataFields) {
        checkFields[field] = new QCheckBox(field, this);
        checkFields[field]->setChecked(true);
        layoutFields.addWidget(checkFields[field]);
    }
    foreach (QString field, requiredFields) {
        if (checkFields.contains(field)) {
            checkFields[field]->setEnabled(false);
        } else {
            QMessageBox::warning(this,
                                 "Warning",
                                 QString("Required field '%1' not found").arg(field));
            ui->boxDataSet->removeItem(ui->boxDataSet->currentIndex());
            return;
        }
    }
    ui->groupField->setLayout(&layoutFields);
    // Show the Dates
    foreach (QString date, checkDates.keys()) {
        layoutDates.removeWidget(checkDates[date]);
        delete checkDates[date];
    }
    checkDates.clear();
    foreach (QString fileName, dataList) {
        QString date = (fileName.simplified().split("_"))[1];
        if (checkDates.contains(date))
            continue;
        checkDates[date] = new QCheckBox(date, this);
        checkDates[date]->setChecked(false);
        layoutDates.addWidget(checkDates[date]);
    }
    ui->groupDate->setLayout(&layoutDates);
}

void LoadWindow::on_buttonLoad_clicked()
{
    QStringList listFields, listDates;
    foreach (QString field, checkFields.keys()) {
        if (checkFields[field]->isChecked())
            listFields.push_back(field);
    }
    foreach (QString date, checkDates.keys()) {
        if (checkDates[date]->isChecked())
            listDates.push_back(date);
    }
    GlobalData::globalData.conf_set("DataPath", ui->boxDataSet->currentText());
    GlobalData::globalData.conf_set("Fields", listFields);
    GlobalData::globalData.conf_set("Dates", listDates);
    GlobalData::globalData.conf_set("Grids",
                                    QStringList{"grid_id",
                                                "vertex0_lat",
                                                "vertex0_lng",
                                                "vertex1_lat",
                                                "vertex1_lng",
                                                "vertex2_lat",
                                                "vertex2_lng",
                                                "vertex3_lat",
                                                "vertex3_lng"});
    QThread *dataBaseInsert = QThread::create([] {
        DataBase::dataBase.init();
        DataBase::dataBase.load();
        DataBase::dataBase.loadGrids();
    });
    connect(dataBaseInsert, &QThread::finished, this, &LoadWindow::insert_finished);
    //    GlobalData::globalData.add_threads(dataBaseInsert);
    dataBaseInsert->start();
}

void LoadWindow::statusText(QString status)
{
    qDebug() << status;
    if (ui->labelStatus->isHidden() == true)
        ui->labelStatus->setHidden(false);
    ui->labelStatus->setText(status);
}
void LoadWindow::statusProgress(int progress)
{
    qDebug() << "Progress:" << progress;
    if (progress > 0) {
        ui->progressBar->setHidden(false);
        ui->progressBar->setValue(progress);
    } else
        ui->progressBar->setHidden(true);
}

void LoadWindow::insert_finished()
{
    qDebug() << "Setting the boxDataSet ...";
    ui->boxDataSet->insertItem(0, ui->boxDataSet->currentText());
    ui->boxDataSet->removeItem(ui->boxDataSet->currentIndex());
    ui->boxDataSet->setCurrentIndex(0);
    qDebug() << "Closing the loadWindow ...";
    this->close();
    emit back_to_mainWindow();
}
