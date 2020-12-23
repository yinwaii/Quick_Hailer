#ifndef LOADWINDOW_H
#define LOADWINDOW_H

#include "Utilities/database.h"
#include "Utilities/globaldata.h"
#include <QCheckBox>
#include <QDebug>
#include <QFileDialog>
#include <QMap>
#include <QMessageBox>
#include <QTextStream>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>

namespace Ui {
class LoadWindow;
}

class LoadWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoadWindow(QWidget *parent = nullptr);
    ~LoadWindow();

signals:
    void back_to_mainWindow();

private slots:
    void on_buttonDataSetOpen_clicked();

    void on_boxDataSet_currentIndexChanged(const QString &arg1);

    void on_buttonLoad_clicked();
    void insert_finished();
public slots:
    void statusText(QString status);
    void statusProgress(int progress);

private:
    Ui::LoadWindow *ui;
    QMap<QString, QCheckBox *> checkFields;
    QVBoxLayout layoutFields;
    QStringList requiredFields
        = {"departure_time", "end_time", "orig_lng", "orig_lat", "dest_lng", "dest_lat", "fee"};
    QMap<QString, QCheckBox *> checkDates;
    QVBoxLayout layoutDates;
};

#endif // LOADWINDOW_H
