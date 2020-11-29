#ifndef PREDICTIONTAB_H
#define PREDICTIONTAB_H

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

private:
    Ui::PredictionTab *ui;
};

#endif // PREDICTIONTAB_H
