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
    connect(ui->pb_showinfo, &QPushButton::clicked,this,&MainWindow::winLicence);
    //timer->start(1000);
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
     QSettings m("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
                 QSettings::NativeFormat);
     QStringList ak = m.childGroups();
     qDebug() << ak;
     m.beginGroup("Autodesk Maya 2014");
     QString path = m.value("UninstallPath").toString();
     m.endGroup();

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

void MainWindow::cpuInfo()
{
    QString system_output;
    QProcess *cmd = new QProcess();
    QString test;
    //system_output = system_output.remove(0,system_output.indexOf("\n")+1)
    //                             .remove(system_output.indexOf("\r"),system_output.length());
        if(QSysInfo::kernelType() == "winnt")
        {
            QStringList cpuname;
            cpuname << "cpu" << "get" << "name";
            cmd->start("wmic", cpuname);
            cmd->waitForStarted();
            cmd->waitForFinished();
            system_output = cmd->readAllStandardOutput();
            system_output = system_output.remove(0,system_output.indexOf("\n")+1)
                                         .remove(system_output.indexOf("\r"),system_output.length());
            qDebug() << "CPU : "  << system_output;
}
}

//void MainWindow::ramInfo()
//{
    //alll
//}

void MainWindow::gpuInfo()
{
    QStringList info_GPU;
    QStringList gpu_RAM;
    QString system_output;
    QString test;
    QProcess *cmd = new QProcess();
    QStringList gpuname;
    gpuname <<"PATH" << "Win32_videocontroller" << "get" << "VideoProcessor";
    cmd->start("wmic",gpuname);
    cmd->waitForStarted();
    cmd->waitForFinished();
    system_output = cmd->readAllStandardOutput();
    system_output = system_output.remove(0,system_output.indexOf("\n")+1);
    info_GPU = system_output.split("\n");
    foreach(QString item,info_GPU){
        item = 0;
        item = item+item;
    }
    qDebug() << "GPU : "  << info_GPU;

    QStringList gpuRam;
    gpuRam << "PATH" << "Win32_VideoController" << "get" << "AdapterRAM";
    cmd->start("wmic",gpuRam);
    cmd->waitForStarted();
    cmd->waitForFinished();
    system_output = cmd->readAllStandardOutput();
    qDebug() << "GPU RAM : "  << system_output;
}


void MainWindow::update()
{
    //call everything
    //storageInfo();
   // sysInfo();
    //networkInfo();
    //biosInfo();
    //cpuInfo();
    //gpuInfo();
    //currentUsage();
}

void MainWindow::currentUsage()
{
    QProcess *command = new QProcess();
    QString CPU;
    QString RAM;
    QString GPU;
    QString script;
    QString script1;
    QStringList args = QString("cpu get loadpercentage").split(' ');
    command->start("wmic",args);
    command->waitForStarted();
    command->waitForFinished();
    CPU = command->readAllStandardOutput();
    CPU = CPU.remove(0,CPU.indexOf("\n")+1).remove(CPU.indexOf(" "),CPU.length());
    qDebug() << "Current usage CPU" << CPU;
    QString path = "C:/Project/NG_2022_System_Manager_project/System_manager/Scripts_powershell/RAM_usage_in_percentage.ps1";
    QFile file(path);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

     while (!file.atEnd()) {
         script.append(file.readLine());
    }
    command->start("powershell",script.split(' '));
    command->waitForStarted();
    command->waitForFinished();
    RAM = command->readAllStandardOutput();
    RAM = RAM.remove(0,RAM.indexOf(":")+2).remove(RAM.indexOf(","),RAM.length());
    qDebug() << "test" << RAM;
    QString path1 = "C:/Project/NG_2022_System_Manager_project/System_manager/Scripts_powershell/Total_GPU_usage_script.ps1";
    QFile file1(path1);
     if (!file1.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

     while (!file1.atEnd()) {
         script1.append(file1.readLine());
    }
     command->start("powershell",script1.split(' '));
     command->waitForStarted();
     command->waitForFinished();
     GPU = command->readAllStandardOutput();
     GPU = GPU.remove(0,GPU.indexOf(":")+2).remove(GPU.indexOf("\r"),GPU.length());
     qDebug() << "Test "<< GPU;
}

void MainWindow::winLicence()
{
    QProcess *cmd = new QProcess();
    QStringList args = QString("slmgr.vbs /dlv").split(' ');
    cmd->start("powershell",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
}
