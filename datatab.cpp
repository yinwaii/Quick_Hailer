#include "datatab.h"
#include "ui_datatab.h"

DataTab::DataTab(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::DataTab)
{
    ui->setupUi(this);
}

DataTab::~DataTab()
{
    delete ui;
}
