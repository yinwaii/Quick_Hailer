#ifndef MAPTAB_H
#define MAPTAB_H

#include "Managers/mapmanager.h"
#include "Utilities/database.h"
#include "Utilities/globaldata.h"
#include <QQmlContext>
#include <QQuickItem>
#include <QWidget>

namespace Ui {
class MapTab;
}

class MapTab : public QWidget
{
    Q_OBJECT

public:
    explicit MapTab(QWidget *parent = nullptr);
    ~MapTab();
public slots:
    void loadMap();
    void loadManeuver(QList<QGeoManeuver> maneuverList);
    void setThermalRange();
    void selectedThermalTime();
    void setFlowRange();
    void selectedFlowTime();
    void getOriginAddress(QString address);
    void getDestinationAddress(QString address);

private slots:
    void on_radioThermalOverall_clicked();

    void on_radioThermalMoment_clicked();

private:
    Ui::MapTab *ui;
    MapManager *managerRoute;
    MapManager *managerThermalOrigin;
    MapManager *managerThermalDestination;
    MapManager *managerFlow;
};

#endif // MAPTAB_H
