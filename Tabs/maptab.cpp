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
}

void MapTab::loadMap()
{
    //    lis = DataBase::dataBase.getGrid();
    //    qDebug() << lis;
    //    emit
    //    managerThermalOrigin->initGrids();
    //    managerThermalOrigin->updateHeatEntry(ui->editThermalTimeFrom->dateTime().toTime_t(),
    //                                          ui->editThermalTimeTo->dateTime().toTime_t(),
    //                                          30);
    //    managerThermalDestination->updateHeatExit(ui->editThermalTimeFrom->dateTime().toTime_t(),
    //                                              ui->editThermalTimeTo->dateTime().toTime_t(),
    //                                              30);
    //    managerFlow->updateRoute(ui->editFlowTimeTo->dateTime().toTime_t(), 50);
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
    }
}

MapTab::~MapTab()
{
    delete ui;
}
