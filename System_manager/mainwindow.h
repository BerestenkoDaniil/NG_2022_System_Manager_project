#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSysInfo>
#include <QStorageInfo>
#include <QNetworkInterface>
#include <QSettings>
#include <QProcess>
#include <QtDebug>
#include <QTimer>
#include <QFile>

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
    void cpuInfo();
    void ramInfo();
    void gpuInfo();
    void update();
    void currentUsage();
    void winLicence();

private:
    Ui::MainWindow *ui;
    QStorageInfo *storage;
    QSysInfo *info;
    QNetworkAddressEntry *network;
    QSettings *settings;
    QProcess *process_system;
    QTimer *timer;

};
#endif // MAINWINDOW_H
