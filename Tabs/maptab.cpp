#include "maptab.h"
#include "ui_maptab.h"

MapTab::MapTab(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::MapTab)
{
    ui->setupUi(this);
}

void MapTab::loadMap()
{
    //    lis = DataBase::dataBase.getGrid();
    //    qDebug() << lis;
    //    emit
    ui->quickWidget->rootObject()->findChild<MapManager *>("mapManager")->initGrids();
}

MapTab::~MapTab()
{
    delete ui;
}
