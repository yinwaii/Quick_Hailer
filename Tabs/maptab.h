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

private:
    Ui::MapTab *ui;
    MapManager *managerRoute;
    MapManager *managerThermalOrigin;
    MapManager *managerThermalDestination;
};

#endif // MAPTAB_H
