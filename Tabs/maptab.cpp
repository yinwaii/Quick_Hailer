#include "maptab.h"
#include "ui_maptab.h"

MapTab::MapTab(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::MapTab)
{
    ui->setupUi(this);
    managerRoute = MapManager::getManager(ui->plotRoute);
    managerThermalOrigin = MapManager::getManager(ui->plotThermalOrigin);
    managerThermalDestination = MapManager::getManager(ui->plotThernalDestination);
    managerFlow = MapManager::getManager(ui->plotFlow);
    connect(ui->buttonRouteOrigin, &QPushButton::clicked, managerRoute, &MapManager::selectFrom);
    connect(ui->buttonRouteDestination, &QPushButton::clicked, managerRoute, &MapManager::selectTo);
    connect(ui->buttonRoutePlanning,
            &QPushButton::clicked,
            managerRoute,
            &MapManager::selectPlanning);
    connect(managerRoute, &MapManager::updateManeuver, this, &MapTab::loadManeuver);
    connect(managerRoute, &MapManager::updateOrigin, this, &MapTab::getOriginAddress);
    connect(managerRoute, &MapManager::updateDestination, this, &MapTab::getDestinationAddress);
}

void MapTab::loadMap()
{
    //    lis = DataBase::dataBase.getGrid();
    //    qDebug() << lis;
    //    emit
    //    managerThermalOrigin->initGrids();
    setThermalRange();
    setFlowRange();
    switch (ui->widgetMap->currentIndex()) {
    case 0:
        if (ui->radioThermalMoment->isChecked()) {
            managerThermalOrigin->updateHeatEntry(ui->sliderThermalTimeLine->value() - 3 * 60,
                                                  ui->sliderThermalTimeLine->value() + 3 * 60,
                                                  ui->spinThermalStep->value());
            managerThermalDestination->updateHeatExit(ui->sliderThermalTimeLine->value() - 3 * 60,
                                                      ui->sliderThermalTimeLine->value() + 3 * 60,
                                                      ui->spinThermalStep->value());
        } else if (ui->radioThermalOverall->isChecked()) {
            managerThermalOrigin->updateHeatEntry(ui->editThermalTimeFrom->dateTime().toTime_t()
                                                      - 3 * 60,
                                                  ui->editThermalTimeTo->dateTime().toTime_t()
                                                      + 3 * 60,
                                                  ui->spinThermalStep->value());
            managerThermalDestination->updateHeatExit(ui->editFlowTimeFrom->dateTime().toTime_t()
                                                          - 3 * 60,
                                                      ui->editFlowTimeTo->dateTime().toTime_t()
                                                          + 3 * 60,
                                                      ui->spinThermalStep->value());
        }
        break;
    case 1:
        managerFlow->updateRoute(ui->sliderFlowTimeLine->value(), ui->spinFlowStep->value());
        break;
    }

    //    managerRoute->updateModel();
    //    managerThermalOrigin->initGrids();
    //    managerThermalDestination->initGrids();
    //    ui->quickWidget->rootObject()->findChild<MapManager *>("mapManager")->initGrids();
    //    ui->plotThermalOrigin->rootObject()->findChild<MapManager *>("mapManager")->initGrids();
    //    ui->plotThermalOrigin->rootObject()
    //        ->findChild<MapManager *>("mapManager")
    //        ->updateHeat(ui->editThermalTimeFrom->dateTime().toTime_t(),
    //                     ui->editThermalTimeTo->dateTime().toTime_t());
}

void MapTab::loadManeuver(QList<QGeoManeuver> maneuverList)
{
    ui->listRoute->clear();
    foreach (QGeoManeuver maneuver, maneuverList) {
        ui->listRoute->addItem(maneuver.instructionText());
        if (maneuver.timeToNextInstruction() > 0) {
            QString instruction = QString("%2 s for %3 m")
                                      .arg(maneuver.timeToNextInstruction())
                                      .arg(maneuver.distanceToNextInstruction());
            ui->listRoute->addItem(instruction);
        }
    }
}

void MapTab::setThermalRange()
{
    ui->sliderThermalTimeLine->setRange(ui->editThermalTimeFrom->dateTime().toTime_t(),
                                        ui->editThermalTimeTo->dateTime().toTime_t());
    ui->sliderThermalTimeLine->setTickInterval((ui->editThermalTimeTo->dateTime().toTime_t()
                                                - ui->editThermalTimeFrom->dateTime().toTime_t())
                                               / ui->spinThermalStepTime->value());
    selectedThermalTime();
}

void MapTab::selectedThermalTime()
{
    ui->labelThermalTime->setText(
        QDateTime::fromTime_t(ui->sliderThermalTimeLine->value()).toString());
    //    loadMap();
}

void MapTab::setFlowRange()
{
    ui->sliderFlowTimeLine->setRange(ui->editFlowTimeFrom->dateTime().toTime_t(),
                                     ui->editFlowTimeTo->dateTime().toTime_t());
    ui->sliderFlowTimeLine->setTickInterval(
        (ui->editFlowTimeTo->dateTime().toTime_t() - ui->editFlowTimeFrom->dateTime().toTime_t())
        / ui->spinFlowStep->value());
    selectedFlowTime();
}

void MapTab::selectedFlowTime()
{
    ui->labelFlowTime->setText(QDateTime::fromTime_t(ui->sliderFlowTimeLine->value()).toString());
}

MapTab::~MapTab()
{
    delete ui;
}

void MapTab::on_radioThermalOverall_clicked()
{
    ui->spinThermalStepTime->setEnabled(false);
    ui->sliderThermalTimeLine->setEnabled(false);
    ui->labelThermalTimeLine->setEnabled(false);
    ui->labelThermalTime->setEnabled(false);
    ui->labelThermalStepTime->setEnabled(false);
}

void MapTab::on_radioThermalMoment_clicked()
{
    ui->spinThermalStepTime->setEnabled(true);
    ui->sliderThermalTimeLine->setEnabled(true);
    ui->labelThermalTimeLine->setEnabled(true);
    ui->labelThermalTime->setEnabled(true);
    ui->labelThermalStepTime->setEnabled(true);
}

void MapTab::getOriginAddress(QString address)
{
    ui->editRouteOrigin->setText(address);
}

void MapTab::getDestinationAddress(QString address)
{
    ui->editRouteDestination->setText(address);
}
