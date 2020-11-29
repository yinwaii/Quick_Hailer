#include "querytab.h"
#include "ui_querytab.h"

QueryTab::QueryTab(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::QueryTab)
{
    ui->setupUi(this);
}

QueryTab::~QueryTab()
{
    delete ui;
}
