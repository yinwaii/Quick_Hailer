#include "Managers/mapmanager.h"
#include "Utilities/globaldata.h"
#include "Windows/loadwindow.h"
#include "Windows/mainwindow.h"

#include <QApplication>
#include <QtQml>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //    qRegisterMetaType<QGeoRoute>("Route");
    //    qRegisterMetaType<QList<MapGrid>>("QList<MapGrid>");
    //    qmlRegisterType<MapGrid>("MapGrid", 1, 0, "MapGrid");
    //    qmlRegisterType<MapGridModel>("MapGridModel", 1, 0, "MapGridModel");
    qmlRegisterType<MapManager>("MapManager", 1, 0, "MapManager");
    //    GlobalData globalData;
    LoadWindow load;
    MainWindow main;
    a.connect(&load, &LoadWindow::back_to_mainWindow, &main, &MainWindow::show);
    a.connect(&load, &LoadWindow::back_to_mainWindow, &main, &MainWindow::open_the_mainWindow);
    a.connect(&GlobalData::globalData,
              &GlobalData::sendStatusText,
              &main,
              &MainWindow::setStatusBar);
    a.connect(&GlobalData::globalData,
              &GlobalData::sendStatusProgress,
              &main,
              &MainWindow::setStatusProgress);
    load.show();
    return a.exec();
}
