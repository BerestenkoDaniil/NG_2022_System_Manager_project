#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QMainWindow>
#include <iostream>
#include <QSysInfo>
#include <QStorageInfo>
#include <QNetworkInterface>
#include <QSettings>
#include <QProcess>
#include <QtDebug>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void storageInfo();
    void sysInfo();
    void networkInfo();
    void biosInfo();
    void cpuIfo();

private:
    Ui::MainWindow *ui;
    QStorageInfo *storage;
    QSysInfo *info;
    QNetworkAddressEntry *network;
    QSettings *settings;
    QProcess *process_system;

};
#endif // MAINWINDOW_H
