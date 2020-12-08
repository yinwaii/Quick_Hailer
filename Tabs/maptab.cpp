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
}

void MapTab::loadMap()
{
    //    lis = DataBase::dataBase.getGrid();
    //    qDebug() << lis;
    //    emit
    //    managerThermalOrigin->initGrids();
    managerThermalOrigin->updateHeatEntry(ui->editThermalTimeFrom->dateTime().toTime_t(),
                                          ui->editThermalTimeTo->dateTime().toTime_t(),
                                          30);
    managerThermalDestination->updateHeatExit(ui->editThermalTimeFrom->dateTime().toTime_t(),
                                              ui->editThermalTimeTo->dateTime().toTime_t(),
                                              30);
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

MapTab::~MapTab()
{
    delete managerRoute;
    delete managerThermalOrigin;
    delete managerThermalDestination;
    delete ui;
}
