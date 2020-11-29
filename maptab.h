#ifndef MAPTAB_H
#define MAPTAB_H

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

private:
    Ui::MapTab *ui;
};

#endif // MAPTAB_H
