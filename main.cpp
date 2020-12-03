#include "Managers/mapmanager.h"
#include "Utilities/globaldata.h"
#include "Windows/loadwindow.h"
#include "Windows/mainwindow.h"

#include <QApplication>
#include <QtQml>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qmlRegisterType<MapManager>("MapManager", 1, 0, "MapManager");
    GlobalData globalData;
    LoadWindow load;
    MainWindow main;
    a.connect(&load, &LoadWindow::back_to_mainWindow, &main, &MainWindow::open_the_mainWindow);
    load.show();
    return a.exec();
}
