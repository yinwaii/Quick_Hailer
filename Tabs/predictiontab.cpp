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
}

PredictionTab::~PredictionTab()
{
    delete ui;
}

void PredictionTab::loadRouteIdeas(QVariantList ideaList)
{
    qDebug() << ideaList;
    ui->listRouteIdea->clear();
    for (int i = 0; i < ideaList.count(); i++) {
        QString tmp = QString("id: %1 time: %2min fee: %3")
                          .arg(i)
                          .arg(ideaList[i].toMap()["time"].toInt() / 60)
                          .arg(ideaList[i].toMap()["fee"].toDouble());
        qDebug() << tmp;
        ui->listRouteIdea->addItem(tmp);
    }
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
