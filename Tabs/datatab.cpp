#include "datatab.h"
#include "ui_datatab.h"

DataTab::DataTab(QWidget *parent) : QWidget(parent), ui(new Ui::DataTab)
{
    ui->setupUi(this);
}

DataTab::~DataTab()
{
    delete ui;
    delete chartDemandTime;
}

void DataTab::printDemandTime()
{
    chartDemandTime = new ChartManager(ui->containerDemandTime);
    chartDemandTime->initSeries("DateTime", {"Entry", "Exit", "Flow"});
    //    QList<QPointF> pointsDemandTime;
    int timeStart = ui->editDataTimeFrom->dateTime().toTime_t();
    int timeStop = ui->editDataTimeTo->dateTime().toTime_t();
    //    DataBase::dataBase.searchNum("SELECT * FROM dataset");
    for (int step = 0; step < ui->spinDataStep->value(); step++) {
        int stepStart = timeStart + (timeStop - timeStart) * step / ui->spinDataStep->value();
        int stepStop = timeStart + (timeStop - timeStart) * (step + 1) / ui->spinDataStep->value();
        int entry = DataBase::dataBase.searchNum(
            QString("SELECT * FROM dataset WHERE departure_time>%1 AND departure_time<%2")
                .arg(stepStart)
                .arg(stepStop));
        int exit = DataBase::dataBase.searchNum(
            QString("SELECT * FROM dataset WHERE end_time>%1 AND end_time<%2")
                .arg(stepStart)
                .arg(stepStop));
        int flow = entry - exit;
        //        qDebug() << double(stepStop - stepStart) << double(entry) / double(stepStop - stepStart);
        chartDemandTime->getSeries()->addCache((stepStart / 2.0 + stepStop / 2.0) * 1000,
                                               {double(entry) * 3600 / (stepStop - stepStart),
                                                double(exit) * 3600 / (stepStop - stepStart),
                                                double(flow) * 3600 / (stepStop - stepStart)});

        //        pointsDemandTime.append(QPointF(stepStart / 2.0 + stepStop / 2.0, result));
    }
    //    chartDemandTime->getAxisX()->setRange(ui->editDataTimeFrom->dateTime().toTime_t(),
    //                                          ui->editDataTimeTo->dateTime().toTime_t());
    chartDemandTime->load();
}

void DataTab::updatePlots()
{
    switch (ui->widgetData->currentIndex()) {
    case 0:
        printDemandTime();
        break;
    }
}
