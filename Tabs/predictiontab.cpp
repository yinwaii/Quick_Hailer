#include "predictiontab.h"
#include "ui_predictiontab.h"

PredictionTab::PredictionTab(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::PredictionTab)
{
    ui->setupUi(this);
    managerRouteBased = MapManager::getManager(ui->containerRouteBased);
    managerSpaceBased = MapManager::getManager(ui->containerSpaceBased);
    connect(ui->buttonRouteOrigin,
            &QPushButton::clicked,
            managerRouteBased,
            &MapManager::selectFrom);
    connect(ui->buttonRouteDestination,
            &QPushButton::clicked,
            managerRouteBased,
            &MapManager::selectTo);
    connect(ui->buttonSpaceOrigin,
            &QPushButton::clicked,
            managerSpaceBased,
            &MapManager::selectFrom);
    connect(managerRouteBased, &MapManager::updateRouteIdeas, this, &PredictionTab::loadRouteIdeas);
    connect(managerSpaceBased, &MapManager::updateSpaceIdeas, this, &PredictionTab::loadSpaceIdeas);
    connect(ui->buttonRoutePredict,
            &QPushButton::clicked,
            managerRouteBased,
            &MapManager::predictRoute);
    connect(managerRouteBased,
            &MapManager::updateOrigin,
            this,
            &PredictionTab::getRouteOriginAddress);
    connect(managerRouteBased,
            &MapManager::updateDestination,
            this,
            &PredictionTab::getRouteDestinationAddress);
    connect(managerSpaceBased,
            &MapManager::updateOrigin,
            this,
            &PredictionTab::getSpaceOriginAddress);
}

PredictionTab::~PredictionTab()
{
    delete ui;
}

void PredictionTab::loadRouteIdeas(QVariantList ideaList)
{
    qDebug() << ideaList;
    ui->listRouteIdea->clear();
    int averageTime = 0;
    double averageFee = 0;
    for (int i = 0; i < ideaList.count(); i++) {
        QString tmp = QString("id: %1 time: %2min fee: %3")
                          .arg(i)
                          .arg(ideaList[i].toMap()["time"].toInt() / 60)
                          .arg(ideaList[i].toMap()["fee"].toDouble());
        averageTime += ideaList[i].toMap()["time"].toInt();
        averageFee += ideaList[i].toMap()["fee"].toDouble();
        qDebug() << tmp;
        ui->listRouteIdea->addItem(tmp);
    }
    averageFee = floor(averageFee * 100 / ideaList.count()) / 100;
    ui->labelAnswer->setText(
        QString("time: %1 min fee: ￥%2").arg(averageTime / (ideaList.count() * 60)).arg(averageFee));
}

void PredictionTab::loadSpaceIdeas(QVariantList ideaList)
{
    qDebug() << ideaList;
    ui->listSpaceIdea->clear();
    for (int i = 0; i < ideaList.count(); i++) {
        QString tmp = QString("id: %1 time: %2min fee: %3")
                          .arg(i)
                          .arg(ideaList[i].toMap()["time"].toInt() / 60)
                          .arg(ideaList[i].toMap()["fee"].toDouble());
        qDebug() << tmp;
        ui->listSpaceIdea->addItem(tmp);
    }
}

void PredictionTab::on_buttonSpacePredict_clicked()
{
    managerSpaceBased->predictSpace(ui->editSpaceTime->text().toInt());
}

void PredictionTab::getRouteOriginAddress(QString address)
{
    ui->editRouteOrigin->setText(address);
}

void PredictionTab::getRouteDestinationAddress(QString address)
{
    ui->editRouteDestination->setText(address);
}

void PredictionTab::getSpaceOriginAddress(QString address)
{
    ui->editSpaceOrigin->setText(address);
}
