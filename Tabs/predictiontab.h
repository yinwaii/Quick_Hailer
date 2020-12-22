#ifndef PREDICTIONTAB_H
#define PREDICTIONTAB_H

#include "Managers/mapmanager.h"
#include <QWidget>

namespace Ui {
class PredictionTab;
}

class PredictionTab : public QWidget
{
    Q_OBJECT

public:
    explicit PredictionTab(QWidget *parent = nullptr);
    ~PredictionTab();

public slots:
    void loadRouteIdeas(QVariantList ideaList);
    void loadSpaceIdeas(QVariantList ideaList);
    void getRouteOriginAddress(QString address);
    void getRouteDestinationAddress(QString address);
    void getSpaceOriginAddress(QString address);

private slots:
    void on_buttonSpacePredict_clicked();

private:
    Ui::PredictionTab *ui;
    MapManager *managerRouteBased;
    MapManager *managerSpaceBased;
};

#endif // PREDICTIONTAB_H
