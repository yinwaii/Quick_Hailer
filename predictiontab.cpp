#include "predictiontab.h"
#include "ui_predictiontab.h"

PredictionTab::PredictionTab(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::PredictionTab)
{
    ui->setupUi(this);
}

PredictionTab::~PredictionTab()
{
    delete ui;
}
