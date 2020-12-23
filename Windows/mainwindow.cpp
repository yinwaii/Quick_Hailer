#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Ride Hailing Data Shower");
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    GlobalData::globalData.statusProgress(0);
    //    progressBar->setAlignment(Qt::AlignRight);
    ui->statusbar->addPermanentWidget(progressBar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open_the_mainWindow()
{
    //    SeriesManager sm;
    //    this->show();
    //    ui->tabData->printDistribution();
    //    ui->tabData->printDemandTime();
    //    ui->tabData->printRevenue();
    //    DataBase::dataBase.getGrid();
    emit ui->tabData->updatePlots();
    emit ui->tabMap->loadMap();
    //    QThread *loadCharts = QThread::create([this] {
    //        ui->tabData->printDistribution();
    //        ui->tabData->printDemandTime();
    //        ui->tabData->printRevenue();
    //    });
    //    loadCharts->start();
}

void MainWindow::setStatusBar(QString status)
{
    ui->statusbar->showMessage(status);
}

void MainWindow::setStatusProgress(int progress)
{
    if (progress <= 0 || progress >= 100)
        progressBar->setHidden(true);
    else {
        progressBar->setHidden(false);
        progressBar->setValue(progress);
    }
}
