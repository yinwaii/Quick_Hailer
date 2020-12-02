#ifndef MAPTAB_H
#define MAPTAB_H

#include "Utilities/database.h"
#include "Utilities/globaldata.h"
#include <QQmlContext>
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
};

#endif // MAPTAB_H
