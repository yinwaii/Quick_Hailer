#include "maptab.h"
#include "ui_maptab.h"

MapTab::MapTab(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::MapTab)
{
    ui->setupUi(this);
}

void MapTab::loadMap() {}

MapTab::~MapTab()
{
    delete ui;
}