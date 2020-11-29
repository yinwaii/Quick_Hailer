#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "database.h"
#include "globaldata.h"
#include <QMainWindow>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void open_the_mainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
