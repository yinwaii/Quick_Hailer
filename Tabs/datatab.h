#ifndef DATATAB_H
#define DATATAB_H

#include "Managers/chartmanager.h"
#include "Utilities/database.h"
#include <QDebug>
#include <QSqlQuery>
#include <QWidget>
#include <QtCharts>

namespace Ui {
class DataTab;
}

class DataTab : public QWidget
{
    Q_OBJECT

public:
    explicit DataTab(QWidget *parent = nullptr);
    ~DataTab();

public slots:
    void printDemandTime();
    void printDistribution();
    void printRevenue();
    void updatePlots();

private:
    Ui::DataTab *ui;
    ChartManager *chartDemandTime;
    ChartManager *chartDistribution;
    ChartManager *chartRevenue;
};

#endif // DATATAB_H
