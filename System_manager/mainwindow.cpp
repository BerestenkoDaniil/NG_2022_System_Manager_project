#include "mainwindow.h"
#include "ui_mainwindow.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cmd = new QProcess();
    timer = new QTimer();
    update();
    connect(timer,&QTimer::timeout,this,&MainWindow::update);
    connect(ui->pb_showinfo, &QPushButton::clicked,this,&MainWindow::winLicence);
    ui->le_name->isReadOnly();
    //timer->start(1000);
    //connect(ui->pb_showinfo,&QPushButton::clicked, this, MainWindow::storageInfo);
    //connect(ui->te_input, &QTextEdit::,this, &MainWindow::storageInfo);
    //moveCursor
    //cpuCoresNum();
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

void MainWindow::installedSoftware_list()
{
    QString programs;
    QString script;
    QString path = "C:/Project/NG_2022_System_Manager_project/System_manager/Scripts_powershell/Storage_Info.ps1";
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    while (!file.atEnd()) {
        script.append(file.readLine());
    }
    cmd->start("powershell",script.split(' '));
    cmd->waitForStarted();
    cmd->waitForFinished();
    programs = cmd->readAllStandardOutput();
    programs = programs.remove(0,25).remove(programs.length()-30,programs.length());
    QStringList prog = programs.split('\n');
    for(int i = 0;i < prog.length();i++){
        if(prog[i].compare(" - ") == 0){
            prog.remove(i);
            i--;
        }
    }
    ui->lw_programs->addItems(prog);
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
    //currentCPU_Usage();
    installedSoftware_list();
}

void MainWindow::currentRAM_Usage()
{
    QString RAM;
    QString script;
    QString path = "C:/Project/NG_2022_System_Manager_project/System_manager/Scripts_powershell/RAM_usage_in_percentage.ps1";
    QFile file(path);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

     while (!file.atEnd()) {
         script.append(file.readLine());
    }
    cmd->start("powershell",script.split(' '));
    cmd->waitForStarted();
    cmd->waitForFinished();
    RAM = cmd->readAllStandardOutput();
    RAM = RAM.remove(0,RAM.indexOf(":")+2).remove(RAM.indexOf(","),RAM.length());
    qDebug() << "test" << RAM;
}

void MainWindow::currentGPU_Usage()
{
    QString GPU;
    QString script;
    QString path = "C:/Project/NG_2022_System_Manager_project/System_manager/Scripts_powershell/Total_GPU_usage_script.ps1";
    QFile file(path);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

     while (!file.atEnd()) {
         script.append(file.readLine());
    }
     cmd->start("powershell",script.split(' '));
     cmd->waitForStarted();
     cmd->waitForFinished();
     GPU = cmd->readAllStandardOutput();
     GPU = GPU.remove(0,GPU.indexOf(":")+2).remove(GPU.indexOf("\r"),GPU.length());
     qDebug() << "Test "<< GPU;
}

void MainWindow::currentCPU_Usage()
{
    QString CPU;
    QStringList args = QString("cpu get loadpercentage").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    CPU = cmd->readAllStandardOutput();
    CPU = CPU.remove(0,CPU.indexOf("\n")+1).remove(CPU.indexOf(" "),CPU.length());
    qDebug() << "Current usage CPU" << CPU;
}

void MainWindow::winLicence()
{
    QStringList args = QString("slmgr.vbs /dlv").split(' ');
    cmd->start("powershell",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
}

void MainWindow::cpuCoresNum()
{
    QString cores;
    QStringList args = QString("cpu get numberofcores").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    cores = cmd->readAllStandardOutput();
    qDebug() << cores;
}

void MainWindow::cpuSocket()
{
    QString socket;
    QStringList args = QString("cpu get SocketDesignation").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    socket = cmd->readAllStandardOutput();
    qDebug() << socket;
}

void MainWindow::cpuThreadcount()
{
    QString thread;
    QStringList args = QString("cpu get ThreadCount").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    thread = cmd->readAllStandardOutput();
    qDebug() << thread;
}

void MainWindow::cpuID()
{
    QString id;
    QStringList args = QString("cpu get ThreadCount").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    id = cmd->readAllStandardOutput();
    qDebug() << id;
}

void MainWindow::gpuDriverver()
{
    QString driver_ver;
    QStringList args = QString("path win32_VideoController get DriverVersion").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    driver_ver = cmd->readAllStandardOutput();
    qDebug() << driver_ver;
}

void MainWindow::gpuCHR()
{
    QString CHR;
    QStringList args = QString("PATH Win32_videocontroller get CurrentHorizontalResolution").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    CHR = cmd->readAllStandardOutput();
    qDebug() << CHR;
}

void MainWindow::gpuCVR()
{
    QString CVR;
    QStringList args = QString("PATH Win32_videocontroller get CurrentVerticalResolution").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    CVR = cmd->readAllStandardOutput();
    qDebug() << CVR;
}

void MainWindow::gpuRefreshrate()
{
    QString rate;
    QStringList args = QString("PATH Win32_videocontroller get CurrentRefreshRate").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    rate = cmd->readAllStandardOutput();
    qDebug() << rate;
}

void MainWindow::gpuMRR()
{
    QString m_rate;
    QStringList args = QString("PATH Win32_videocontroller get MaxRefreshRate").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    m_rate = cmd->readAllStandardOutput();
    qDebug() << m_rate;
}

void MainWindow::gpuDACT()
{
    QString dact;
    QStringList args = QString("PATH Win32_videocontroller get AdapterDACType").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    dact = cmd->readAllStandardOutput();
    qDebug() << dact;
}

void MainWindow::ramCapacity()
{
    QString capacity;
    QStringList args = QString("MEMORYCHIP get Capacity").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    capacity = cmd->readAllStandardOutput();
    qDebug() << capacity;
}

void MainWindow::ramSpeed()
{
    QString speed;
    QStringList args = QString("MEMORYCHIP get Speed").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    speed = cmd->readAllStandardOutput();
    qDebug() << speed;
}

void MainWindow::ramManufecturer()
{
    QString manufacturer;
    QStringList args = QString("MEMORYCHIP get Manufacturer").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    manufacturer = cmd->readAllStandardOutput();
    qDebug() << manufacturer;
}

void MainWindow::ramVoltage()
{
    QString voltage;
    QStringList args = QString("MEMORYCHIP get Manufacturer").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    voltage = cmd->readAllStandardOutput();
    qDebug() << voltage;
}
