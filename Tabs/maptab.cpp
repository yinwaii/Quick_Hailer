#include "maptab.h"
#include "ui_maptab.h"

MapTab::MapTab(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::MapTab)
{
    ui->setupUi(this);
    managerRoute = MapManager::getMapManager(ui->plotRoute);
    managerThermalOrigin = MapManager::getMapManager(ui->plotThermalOrigin);
    managerThermalDestination = MapManager::getMapManager(ui->plotThernalDestination);
}

void MapTab::loadMap()
{
    //    lis = DataBase::dataBase.getGrid();
    //    qDebug() << lis;
    //    emit
    managerRoute->initGrids();
    managerRoute->updateHeat(ui->editThermalTimeFrom->dateTime().toTime_t(),
                             ui->editThermalTimeTo->dateTime().toTime_t());
    //    managerRoute->updateModel();
    managerThermalOrigin->initGrids();
    //    managerThermalDestination->initGrids();
    //    ui->quickWidget->rootObject()->findChild<MapManager *>("mapManager")->initGrids();
    //    ui->plotThermalOrigin->rootObject()->findChild<MapManager *>("mapManager")->initGrids();
    //    ui->plotThermalOrigin->rootObject()
    //        ->findChild<MapManager *>("mapManager")
    //        ->updateHeat(ui->editThermalTimeFrom->dateTime().toTime_t(),
    //                     ui->editThermalTimeTo->dateTime().toTime_t());
}

MapTab::~MapTab()
{
    delete ui;
}
