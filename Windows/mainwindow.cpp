#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open_the_mainWindow()
{
    SeriesManager sm;
    this->show();
    ui->tabData->printDistribution();
    ui->tabData->printDemandTime();
    ui->tabData->printRevenue();
    //    QThread *loadCharts = QThread::create([this] {
    //        ui->tabData->printDistribution();
    //        ui->tabData->printDemandTime();
    //        ui->tabData->printRevenue();
    //    });
    //    loadCharts->start();
}
