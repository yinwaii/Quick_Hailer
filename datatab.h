#ifndef DATATAB_H
#define DATATAB_H

#include <QWidget>

namespace Ui {
class DataTab;
}

class DataTab : public QWidget
{
    Q_OBJECT

public:
    explicit DataTab(QWidget *parent = nullptr);
    ~DataTab();

private:
    Ui::DataTab *ui;
};

#endif // DATATAB_H
