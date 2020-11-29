#include "globaldata.h"
#include "loadwindow.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GlobalData globalData;
    LoadWindow load;
    MainWindow main;
    a.connect(&load, &LoadWindow::back_to_mainWindow, &main, &MainWindow::open_the_mainWindow);
    load.show();
    return a.exec();
}