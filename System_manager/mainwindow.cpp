#include "mainwindow.h"
#include "ui_mainwindow.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    process_system = new QProcess();
    timer = new QTimer();
    update();
    connect(timer,&QTimer::timeout,this,&MainWindow::update);
    timer->start(1000);
    //connect(ui->pb_showinfo,&QPushButton::clicked, this, MainWindow::storageInfo);
    //connect(ui->te_input, &QTextEdit::,this, &MainWindow::storageInfo);
    //moveCursor
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::storageInfo()
{
    ui->le_name->setText("Name");
    ui->le_systype->setText("System Type");
    ui->le_size->setText("Storage size");
    ui->le_asize->setText("Avaible size");
     QStorageInfo storage = QStorageInfo::root();

     qDebug() << storage.rootPath();
     if (storage.isReadOnly())
              qDebug() << "isReadOnly:" << storage.isReadOnly();
     ui->le_sname->setText(storage.rootPath());
     ui->le_syst->setText(storage.fileSystemType());
     ui->le_ssize->setText(QString::number(storage.bytesTotal()/1000000000)+"GB");
     ui->le_assize->setText(QString::number(storage.bytesAvailable()/1000000000)+"GB");
     qDebug() << "name:" << storage.name();
     qDebug() << "fileSystemType:" << storage.fileSystemType();
     qDebug() << "size:" << storage.bytesTotal()/1000000000  << "GB";
     qDebug() << "availableSize:" << storage.bytesAvailable()/1000000000  << "GB";
}

void MainWindow::sysInfo()
{
    qDebug() << "CPU Architecture : " << QSysInfo::currentCpuArchitecture().toLocal8Bit().constData();
    qDebug() << "Product Type : " << QSysInfo::prettyProductName().toLocal8Bit().constData();
    qDebug() << "Kernel Type : " << QSysInfo::kernelType().toLocal8Bit().constData();
    qDebug() << "Kernel Version : " << QSysInfo::kernelVersion().toLocal8Bit().constData();
    qDebug() << "Machine ID : " << QSysInfo::machineHostName().toLocal8Bit().constData();
}

void MainWindow::networkInfo()
{
    qDebug() << "Connected Network Informations";

    foreach(QNetworkInterface networkInterface, QNetworkInterface::allInterfaces())
    {
        if (networkInterface.flags().testFlag(QNetworkInterface::IsUp) && !networkInterface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            foreach (QNetworkAddressEntry entry, networkInterface.addressEntries())
            {
                if ( entry.ip().toString().contains(".")){
                    qDebug() << "Interface:"<< networkInterface.name().toLocal8Bit().constData() << " ";
                    qDebug() << "IP:"<< entry.ip().toString().toLocal8Bit().constData() << " ";
                    qDebug() << "MAC:"  << networkInterface.hardwareAddress().toLocal8Bit().constData();
                }
            }
        }
    }
}

void MainWindow::biosInfo()
{
    qDebug() << "System BIOS informations. ONLY FOR WINDOWS.";

    QSettings settings("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\BIOS", QSettings::NativeFormat);
    qDebug() << "Base Manufacturer : " << settings.value("BaseBoardManufacturer", "0").toString().toLocal8Bit().constData();
    qDebug() << "Base Product : " << settings.value("BaseBoardProduct", "0").toString().toLocal8Bit().constData();
    qDebug() << "BIOS Vendor : " << settings.value("BIOSVendor", "0").toString().toLocal8Bit().constData();
    qDebug() << "BIOS Release Date : " << settings.value("BIOSReleaseDate", "0").toString().toLocal8Bit().constData();
    qDebug() << "System Manufacturer : " << settings.value("SystemManufacturer", "0").toString().toLocal8Bit().constData();
    qDebug() << "Product Name : " << settings.value("SystemProductName", "0").toString().toLocal8Bit().constData();
    qDebug() << "System SKU : " << settings.value("SystemSKU", "0").toString().toLocal8Bit().constData();
}

void MainWindow::cpuIfo()
{
    QString system_output;
    QStringList info_GPU;
    QStringList gpu_RAM;
    QProcess *cmd1 = new QProcess();
    QProcess *cmd2 = new QProcess();
    QProcess *cmd3 = new QProcess();
    QString test;
    //system_output = system_output.remove(0,system_output.indexOf("\n")+1)
    //                             .remove(system_output.indexOf("\r"),system_output.length());
        if(QSysInfo::kernelType() == "winnt")
        {
            QStringList cpuname;
            cpuname << "cpu" << "get" << "name";
            cmd1->start("wmic", cpuname);
            cmd1->waitForStarted();
            cmd1->waitForFinished();
            system_output = cmd1->readAllStandardOutput();
            system_output = system_output.remove(0,system_output.indexOf("\n")+1)
                                         .remove(system_output.indexOf("\r"),system_output.length());
            qDebug() << "CPU : "  << system_output;

            QStringList gpuname;
            gpuname <<"PATH" << "Win32_videocontroller" << "get" << "VideoProcessor";
            cmd2->start("wmic",gpuname);
            cmd2->waitForStarted();
            cmd2->waitForFinished();
            system_output = cmd2->readAllStandardOutput();
            system_output = system_output.remove(0,system_output.indexOf("\n")+1);
            info_GPU = system_output.split("\n");
            foreach(QString item,info_GPU){
                item = 0;
                item = item+item;
            }
            qDebug() << "GPU : "  << info_GPU;

            QStringList gpuRam;
            gpuRam << "PATH" << "Win32_VideoController" << "get" << "AdapterRAM";
            cmd3->start("wmic",gpuRam);
            cmd3->waitForStarted();
            cmd3->waitForFinished();
            system_output = cmd3->readAllStandardOutput();
            qDebug() << "GPU RAM : "  << system_output;
        }
}

void MainWindow::update()
{
    //call everything
    storageInfo();
    sysInfo();
    networkInfo();
    biosInfo();
    cpuIfo();
}
