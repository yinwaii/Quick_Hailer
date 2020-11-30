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
    chartDemandTime->initSeries("DateTime", {"Number"});
    QList<QPointF> pointsDemandTime;
    int timeStart = ui->editDataTimeFrom->dateTime().toTime_t();
    int timeStop = ui->editDataTimeTo->dateTime().toTime_t();
    //    DataBase::dataBase.searchNum("SELECT * FROM dataset");
    for (int step = 0; step < ui->spinDataStep->value(); step++) {
        int stepStart = timeStart + (timeStop - timeStart) * step / ui->spinDataStep->value();
        int stepStop = timeStart + (timeStop - timeStart) * (step + 1) / ui->spinDataStep->value();
        int result = DataBase::dataBase.searchNum(
            QString("SELECT * FROM dataset WHERE departure_time>%1 AND end_time<%2")
                .arg(stepStart)
                .arg(stepStop));
        chartDemandTime->getSeries()->addCache(stepStart / 2.0 + stepStop / 2.0, {double(result)});

        //        pointsDemandTime.append(QPointF(stepStart / 2.0 + stepStop / 2.0, result));
    }
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
