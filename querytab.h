#ifndef QUERYTAB_H
#define QUERYTAB_H

#include <QWidget>

namespace Ui {
class QueryTab;
}

class QueryTab : public QWidget
{
    Q_OBJECT

public:
    explicit QueryTab(QWidget *parent = nullptr);
    ~QueryTab();

private:
    Ui::QueryTab *ui;
};

#endif // QUERYTAB_H
