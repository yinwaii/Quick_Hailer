#include "datatab.h"
#include "ui_datatab.h"

DataTab::DataTab(QWidget *parent) : QWidget(parent), ui(new Ui::DataTab)
{
    ui->setupUi(this);
    managerGrid = MapManager::getManager(ui->containerGrid);
    managerGrid->setFullGrid();
    //    connect(managerGrid, &MapManager::updateDemandPlot, this, &DataTab::printDemandTime);
}

DataTab::~DataTab()
{
    delete ui;
    //    delete chartDemandTime;
}

void DataTab::printDemandTime()
{
    chartDemandTime = new ChartManager(ui->containerDemandTime);
    chartDemandTime->addSeriesId("DateTime", {"Entry", "Exit", "Flow"});
    chartDemandTime->initAxisX(AxisManager::AxisType::DataTime,
                               Qt::AlignBottom,
                               AxisManager::internal_date);
    chartDemandTime->addAxis("Flow", AxisManager::AxisType::Value, Qt::AlignRight);
    //    QList<QPointF> pointsDemandTime;
    QThread *printDemandChart = QThread::create([this] {
        int timeStart = ui->editDataTimeFrom->dateTime().toTime_t();
        int timeStop = ui->editDataTimeTo->dateTime().toTime_t();
        QVariantList dataDemand = DataBase::dataBase.searchDemand(timeStart,
                                                                  timeStop,
                                                                  ui->spinDataStep->value(),
                                                                  managerGrid->gridSelected());
        qDebug() << dataDemand;
        //    DataBase::dataBase.searchNum("SELECT * FROM dataset");
        for (int step = 0; step < ui->spinDataStep->value(); step++) {
            int stepStart = timeStart + (timeStop - timeStart) * step / ui->spinDataStep->value();
            int stepStop = timeStart
                           + (timeStop - timeStart) * (step + 1) / ui->spinDataStep->value();
            int entry = dataDemand[step].toMap()["entry"].toInt();
            int exit = dataDemand[step].toMap()["exit"].toInt();
            int flow = entry - exit;
            //        qDebug() << double(stepStop - stepStart) << double(entry) / double(stepStop - stepStart);
            chartDemandTime->getSeries()->addCache("DateTime",
                                                   (stepStart / 2.0 + stepStop / 2.0) * 1000,
                                                   {double(entry) * 3600 / (stepStop - stepStart),
                                                    double(exit) * 3600 / (stepStop - stepStart),
                                                    double(flow) * 3600 / (stepStop - stepStart)});

            //        pointsDemandTime.append(QPointF(stepStart / 2.0 + stepStop / 2.0, result));
        }
    });
    connect(printDemandChart, &QThread::finished, chartDemandTime, &ChartManager::load);
    connect(printDemandChart, &QThread::finished, [printDemandChart] {
        GlobalData::globalData.remove_threads(printDemandChart);
    });
    //    GlobalData::globalData.clear_threads();
    GlobalData::globalData.add_threads(printDemandChart);
    printDemandChart->start();
    //    chartDemandTime->getAxisX()->setRange(ui->editDataTimeFrom->dateTime().toTime_t(),
    //                                          ui->editDataTimeTo->dateTime().toTime_t());
}

void DataTab::printDistribution()
{
    chartDistribution = new ChartManager(ui->containerDistributionTime);
    chartDistribution->addSeriesId("Time", {"Travel Time Distribution", "Travel Time Accumulation"});
    chartDistribution->addSeriesId("Fee", {"Order Fees Distribution", "Order Fees Accumulation"});
    chartDistribution->addAxis("Accumulation",
                               AxisManager::AxisType::Value,
                               Qt::AlignRight,
                               {"Travel Time Accumulation", "Order Fees Accumulation"});
    chartDistribution->addAxis("Fee",
                               AxisManager::AxisType::Value,
                               Qt::AlignTop,
                               {"Order Fees Distribution", "Order Fees Accumulation"});
    chartDistribution->getAxisX()->setTickCount(12);
    chartDistribution->getOtherAxis("Fee")->setTickCount(12);
    QThread *printDistributionChart = QThread::create([this] {
        int wholenum = DataBase::dataBase.searchNum(
            QString("SELECT * From dataset WHERE "
                    "departure_time>%1 AND end_time<%2")
                .arg(ui->editDataTimeFrom->dateTime().toTime_t())
                .arg(ui->editDataTimeTo->dateTime().toTime_t()));
        double maxTime = double(
            DataBase::dataBase
                .searchTarget(QString("SELECT MAX(time) As Target From dataset WHERE "
                                      "departure_time>%1 AND end_time<%2")
                                  .arg(ui->editDataTimeFrom->dateTime().toTime_t())
                                  .arg(ui->editDataTimeTo->dateTime().toTime_t()))
                .toInt());
        double maxFee = double(
            DataBase::dataBase
                .searchTarget(QString("SELECT MAX(fee) As Target From dataset WHERE "
                                      "departure_time>%1 AND end_time<%2")
                                  .arg(ui->editDataTimeFrom->dateTime().toTime_t())
                                  .arg(ui->editDataTimeTo->dateTime().toTime_t()))
                .toInt());
        double timeCount = 0;
        double feeCount = 0;
        double changedstep = double(ui->spinDataStep->value()) / double(80) * double(300);
        QVariantList dataDistribution
            = DataBase::dataBase.searchDistribution(ui->editDataTimeFrom->dateTime().toTime_t(),
                                                    ui->editDataTimeTo->dateTime().toTime_t(),
                                                    maxTime,
                                                    maxFee,
                                                    changedstep);
        qDebug() << dataDistribution;
        for (int step = 0; step < changedstep; step++) {
            double timeStart = maxTime * double(step) / changedstep;
            double timeStop = maxTime * double(step + 1) / changedstep;
            double feeStart = maxFee * double(step) / changedstep;
            double feeStop = maxFee * double(step + 1) / changedstep;
            //        int numberTime = DataBase::dataBase.searchNum(
            //            QString("SELECT * FROM dataset WHERE time>%1 AND time<%2 AND "
            //                    "departure_time>%3 AND departure_time<%4")
            //                .arg(timeStart)
            //                .arg(timeStop)
            //                .arg(ui->editDataTimeFrom->dateTime().toTime_t())
            //                .arg(ui->editDataTimeTo->dateTime().toTime_t()));
            //        int numberFee = DataBase::dataBase.searchNum(
            //            QString("SELECT * FROM dataset WHERE fee>%1 AND fee<%2 AND "
            //                    "departure_time>%3 AND departure_time<%4")
            //                .arg(feeStart)
            //                .arg(feeStop)
            //                .arg(ui->editDataTimeFrom->dateTime().toTime_t())
            //                .arg(ui->editDataTimeTo->dateTime().toTime_t()));
            int numberTime = dataDistribution[step].toMap()["time"].toInt();
            int numberFee = dataDistribution[step].toMap()["fee"].toInt();
            //        qDebug() << numberTime << numberTime_1 << numberFee << numberFee_1;
            timeCount += numberTime;
            feeCount += numberFee;
            //        qDebug() << feeCount;
            chartDistribution->getSeries()->addCache("Time",
                                                     (timeStart + timeStop) / 120,
                                                     {
                                                         double(numberTime) / wholenum * 100,
                                                         double(timeCount) / wholenum * 100,
                                                     });
            chartDistribution->getSeries()->addCache("Fee",
                                                     (feeStart + feeStop) / 2,
                                                     {double(numberFee) / wholenum * 100,
                                                      double(feeCount) / wholenum * 100});
            if ((double(timeCount) / wholenum * 100) > 99
                || (double(feeCount) / wholenum * 100) > 99) {
                break;
            }
        }
    });
    connect(printDistributionChart, &QThread::finished, chartDistribution, &ChartManager::load);
    connect(printDistributionChart, &QThread::finished, [printDistributionChart] {
        GlobalData::globalData.remove_threads(printDistributionChart);
    });
    //    qDebug() << maxTime - rightbound;
    //    GlobalData::globalData.clear_threads();
    GlobalData::globalData.add_threads(printDistributionChart);
    printDistributionChart->start();
    //    chartDistributionFee->load();
}

void DataTab::printRevenue()
{
    chartRevenue = new ChartManager(ui->containerRevenue);
    chartRevenue->addSeriesId("DateTime", {"Revenue"});
    chartRevenue->initAxisX(AxisManager::AxisType::DataTime,
                            Qt::AlignBottom,
                            AxisManager::internal_date);
    QThread *printRevenueChart = QThread::create([this] {
        int timeStart = ui->editDataTimeFrom->dateTime().toTime_t();
        int timeStop = ui->editDataTimeTo->dateTime().toTime_t();
        QVariantList dataRevenue = DataBase::dataBase.searchRevenue(timeStart,
                                                                    timeStop,
                                                                    ui->spinDataStep->value());
        for (int step = 0; step < ui->spinDataStep->value(); step++) {
            int stepStart = timeStart + (timeStop - timeStart) * step / ui->spinDataStep->value();
            int stepStop = timeStart
                           + (timeStop - timeStart) * (step + 1) / ui->spinDataStep->value();
            double revenue = dataRevenue[step].toMap()["fee"].toDouble();
            chartRevenue->getSeries()->addCache("DateTime",
                                                (stepStart / 2.0 + stepStop / 2.0) * 1000,
                                                {double(revenue) * 3600 / (stepStop - stepStart)});
        }
    });
    connect(printRevenueChart, &QThread::finished, chartRevenue, &ChartManager::load);
    connect(printRevenueChart, &QThread::finished, [printRevenueChart] {
        GlobalData::globalData.remove_threads(printRevenueChart);
    });
    //    GlobalData::globalData.clear_threads();
    GlobalData::globalData.add_threads(printRevenueChart);
    printRevenueChart->start();
}

void DataTab::updatePlots()
{
    if (GlobalData::globalData.isEmpty_threads()) {
        managerGrid->initGrids();
        qDebug() << managerGrid->gridList();
        GlobalData::globalData.clear_threads();
        switch (ui->widgetData->currentIndex()) {
        case 0:
            printDemandTime();
            break;
        case 1:
            printDistribution();
            break;
        case 2:
            printRevenue();
        }
        if (ui->widgetData->currentIndex() != 0)
            printDemandTime();
        if (ui->widgetData->currentIndex() != 1)
            printDistribution();
        if (ui->widgetData->currentIndex() != 2)
            printRevenue();
    }
    //    QThread *printCharts = QThread::create([this] {
    //        switch (ui->widgetData->currentIndex()) {
    //        case 0:
    //            printDemandTime();
    //            break;
    //        case 1:
    //            printDistribution();
    //            break;
    //        case 2:
    //            printRevenue();
    //        }
    //    });
    //    printCharts->start();
}

void DataTab::on_pushButton_2_clicked()
{
    if (GlobalData::globalData.isEmpty_threads())
        printDemandTime();
}

void DataTab::on_pushButton_clicked()
{
    managerGrid->setFullGrid();
    managerGrid->initGrids();
}

void DataTab::on_widgetData_currentChanged(int index)
{
    if (GlobalData::globalData.isEmpty_threads()) {
        switch (index) {
        case 0:
            chartDemandTime->show();
            break;
        case 1:
            chartDistribution->show();
            break;
        case 2:
            chartRevenue->show();
            break;
        }
    }
}
