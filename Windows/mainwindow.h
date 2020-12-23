#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Managers/seriesmanager.h"
#include "Utilities/database.h"
#include "Utilities/globaldata.h"
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
    void open_the_mainWindow();
public slots:
    void setStatusBar(QString status);
    void setStatusProgress(int progress);

private:
    Ui::MainWindow *ui;
    QProgressBar *progressBar;
};
#endif // MAINWINDOW_H
