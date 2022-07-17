#include "mainwindow.h"
#include "ui_mainwindow.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::setWindowFlag(Qt::WindowMaximizeButtonHint,false);
    cmd = new QProcess();
    command = new QProcess();
    timer = new QTimer();
    update();
    connect(timer,&QTimer::timeout,this,&MainWindow::update);
    connect(ui->pb_showinfo, &QPushButton::clicked,this,&MainWindow::winLicence);
    timer->start(1000);
    currentGPU_Usage();
    currentCPU_Usage();
    currentRAM_Usage();
    storageInfo();
    installedSoftware_list();
    networkInfo();
    biosInfo();
    sysInfo();
    ramInfo();
    CPU();
    GPU();
    Motherboard();
    gpuInfo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::storageInfo()
{
    QStorageInfo storage = QStorageInfo::root();
    if (storage.isReadOnly())
        qDebug() << "isReadOnly:" << storage.isReadOnly();
    ui->le_sname->setReadOnly(true);
    ui->le_size->setReadOnly(true);
    ui->le_asize->setReadOnly(true);
    ui->le_systype->setReadOnly(true);
    ui->le_sname->setText(storage.rootPath());
    ui->le_size->setText(QString::number(storage.bytesTotal()/1000000000)+"GB");
    ui->le_asize->setText(QString::number(storage.bytesAvailable()/1000000000)+"GB");
    ui->le_systype->setText(storage.fileSystemType());
}

void MainWindow::CPU()
{
    cpuInfo();
    cpuCoresNum();
    cpuSocket();
    cpuThreadcount();
    cpuID();

}

void MainWindow::GPU()
{
    gpuDriverver();
    gpuRAM();
    gpuCHR();
    gpuCVR();
    gpuRefreshrate();
    gpuMRR();
    gpuDACT();
}

void MainWindow::Motherboard()
{
    motherbordVersion();
    motherboardSerialnum();
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
    ui->le_cpuarch->setText(QSysInfo::currentCpuArchitecture().toLocal8Bit().constData());
    ui->le_ptype->setText(QSysInfo::prettyProductName().toLocal8Bit().constData());
    ui->le_kernelt->setText(QSysInfo::kernelType().toLocal8Bit().constData());
    ui->le_kernelv->setText(QSysInfo::kernelVersion().toLocal8Bit().constData());
    ui->le_machineid->setText(QSysInfo::machineHostName().toLocal8Bit().constData());
}

void MainWindow::networkInfo()
{
    ui->le_interface->setReadOnly(true);
    ui->le_ip->setReadOnly(true);
    ui->le_MAC->setReadOnly(true);
    foreach(QNetworkInterface networkInterface, QNetworkInterface::allInterfaces())
    {
        if (networkInterface.flags().testFlag(QNetworkInterface::IsUp) && !networkInterface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            foreach (QNetworkAddressEntry entry, networkInterface.addressEntries())
            {
                if ( entry.ip().toString().contains(".")){
                    ui->le_interface->setText(networkInterface.name().toLocal8Bit().constData());
                    ui->le_ip->setText(entry.ip().toString().toLocal8Bit().constData());
                    ui->le_MAC->setText(networkInterface.hardwareAddress().toLocal8Bit().constData());
                }
            }
        }
    }
}

void MainWindow::biosInfo()
{
    QSettings settings("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\BIOS", QSettings::NativeFormat);
    ui->le_basemanuf->setText(settings.value("BaseBoardManufacturer", "0").toString().toLocal8Bit().constData());
    ui->le_baseprod->setText(settings.value("BaseBoardProduct", "0").toString().toLocal8Bit().constData());
    ui->le_biosv->setText(settings.value("BIOSVendor", "0").toString().toLocal8Bit().constData());
    ui->le_biosdate->setText(settings.value("BIOSReleaseDate", "0").toString().toLocal8Bit().constData());
    ui->le_sysymanuf->setText(settings.value("SystemManufacturer", "0").toString().toLocal8Bit().constData());
    ui->le_prodname->setText(settings.value("SystemProductName", "0").toString().toLocal8Bit().constData());
    ui->le_sysSKU->setText(settings.value("SystemSKU", "0").toString().toLocal8Bit().constData());
}

void MainWindow::cpuInfo()
{
    QString system_output;
    QString test;
    if(QSysInfo::kernelType() == "winnt"){
        QStringList cpuname;
        cpuname << "cpu" << "get" << "name";
        cmd->start("wmic", cpuname);
        cmd->waitForStarted();
        cmd->waitForFinished();
        system_output = cmd->readAllStandardOutput();
        system_output = system_output.remove(0,system_output.indexOf("\n")+1)
                                     .remove(system_output.indexOf("\r"),system_output.length());
        ui->le_name->setText(system_output);
    }
}

void MainWindow::ramInfo()
{
    ramCapacity();
    ramSpeed();
    ramManufecturer();
    ramVoltage();
    ramPostion();
}

void MainWindow::gpuInfo()
{
    QStringList info_GPU;
    QString system_output;
    QStringList gpuname;
    gpuname <<"PATH" << "Win32_videocontroller" << "get" << "VideoProcessor";
    cmd->start("wmic",gpuname);
    cmd->waitForStarted();
    cmd->waitForFinished();
    system_output = cmd->readAllStandardOutput();
    system_output = system_output.remove(0,system_output.indexOf("\n")+1);
    info_GPU = system_output.split("\n");
    for(int i = 0;i < info_GPU.length();i++){
        info_GPU[i] = info_GPU[i].remove(info_GPU[i].indexOf("\r"),info_GPU.length());
        if(info_GPU[i].compare("") == 0){
            info_GPU.remove(i);
            i--;
        }
    }
    ui->lw_name->addItems(info_GPU);
}

void MainWindow::gpuRAM()
{
    QString gpuram;
    QStringList gpuRam_args;
    gpuRam_args << "PATH" << "Win32_VideoController" << "get" << "AdapterRAM";
    cmd->start("wmic",gpuRam_args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    gpuram = cmd->readAllStandardOutput();
    gpuram = gpuram.remove(0,gpuram.indexOf("\n")+1).remove(gpuram.length()-7,7);
    QStringList GPURAM = gpuram.split("\n");
    for(int i = 0;i < GPURAM.length();i++){
        GPURAM[i] = GPURAM[i].remove(" ");
        double size = GPURAM[i].toDouble();
        size /= qPow(2,30);
        GPURAM[i] = QString::number(size) +" GB";
    }
    ui->lw_gpuram->addItems(GPURAM);
}

void MainWindow::update()
{
    switch (ui->tabWidget->currentIndex()) {
    case 0:
        currentCPU_Usage();
        break;
    case 1:
        currentGPU_Usage();
        break;
    case 2:
        currentRAM_Usage();
        break;
    }
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
    ui->le_ramusage->setReadOnly(true);
    ui->le_ramusage->setText(RAM+"%");
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
    ui->le_gpuusage->setReadOnly(true);
    ui->le_gpuusage->setText(GPU);
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
    ui->le_usage->setReadOnly(true);
    ui->le_usage->setText(CPU+"%");
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
    cores = cores.remove(0,cores.indexOf("\n")+1).remove(cores.indexOf(" "),cores.length());
    ui->le_cores->setReadOnly(true);
    ui->le_cores->setText(cores);
}

void MainWindow::cpuSocket()
{
    QString socket;
    QStringList args = QString("cpu get SocketDesignation").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    socket = cmd->readAllStandardOutput();
    socket = socket.remove(0,socket.indexOf("\n")+1).remove(socket.indexOf(" "),socket.length());
    ui->le_socket->setReadOnly(true);
    ui->le_socket->setText(socket);
}

void MainWindow::cpuThreadcount()
{
    QString thread;
    QStringList args = QString("cpu get ThreadCount").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    thread = cmd->readAllStandardOutput();
    thread = thread.remove(0,thread.indexOf("\n")+1).remove(thread.indexOf(" "),thread.length());
    ui->le_thread->setReadOnly(true);
    ui->le_thread->setText(thread);
}

void MainWindow::cpuID()
{
    QString id;
    QStringList args = QString("cpu get ProcessorID").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    id = cmd->readAllStandardOutput();
    id = id.remove(0,id.indexOf("\n")+1).remove(id.indexOf(" "),id.length());
    ui->le_id->setReadOnly(true);
    ui->le_id->setText(id);
}

void MainWindow::gpuDriverver()
{
    QString driver_ver;
    QStringList args = QString("path win32_VideoController get DriverVersion").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    driver_ver = cmd->readAllStandardOutput();
    driver_ver = driver_ver.remove(0,driver_ver.indexOf("\n")).remove(driver_ver.length()-7,6);
    QStringList versions = driver_ver.split("\n");
    for(int i = 0;i < versions.length();i++){
        versions[i] = versions[i].remove(versions[i].length()-1,versions[i].indexOf("\n"));
        if(versions[i].compare("") == 0){
            versions.remove(i);
            i--;
        }
    }
    ui->lw_version->addItems(versions);
}

void MainWindow::gpuCHR()
{
    QString CHR;
    QStringList args = QString("PATH Win32_videocontroller get CurrentHorizontalResolution").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    CHR = cmd->readAllStandardOutput();
    CHR = CHR.remove(0,CHR.indexOf("\n")+1).remove(CHR.indexOf(" "),CHR.length());
    ui->le_horizontal->setReadOnly(true);
    ui->le_horizontal->setText(CHR);
}

void MainWindow::gpuCVR()
{
    QString CVR;
    QStringList args = QString("PATH Win32_videocontroller get CurrentVerticalResolution").split(' ');
    cmd->start("wmic",args);
    cmd->waitForStarted();
    cmd->waitForFinished();
    CVR = cmd->readAllStandardOutput();
    CVR = CVR.remove(0,CVR.indexOf("\n")+1).remove(CVR.indexOf(" "),CVR.length());
    ui->le_vertical->setReadOnly(true);
    ui->le_vertical->setText(CVR);
}

void MainWindow::gpuRefreshrate()
{
    QString rate;
    QStringList args = QString("PATH Win32_videocontroller get CurrentRefreshRate").split(' ');
    command->start("wmic",args);
    command->waitForStarted();
    command->waitForFinished();
    rate = command->readAllStandardOutput();
    rate = rate.remove(0,rate.indexOf("\n")+1).remove(rate.indexOf(" "),rate.length());
    ui->le_refreashrate->setReadOnly(true);
    ui->le_refreashrate->setText(rate);
}

void MainWindow::gpuMRR()
{
    QString m_rate;
    QStringList args = QString("PATH Win32_videocontroller get MaxRefreshRate").split(' ');
    command->start("wmic",args);
    command->waitForStarted();
    command->waitForFinished();
    m_rate = command->readAllStandardOutput();
    m_rate = m_rate.remove(0,m_rate.indexOf("\n")+1).remove(m_rate.indexOf(" "),m_rate.length());
    ui->le_maxrefreashrate_2->setReadOnly(true);
    ui->le_maxrefreashrate_2->setText(m_rate);
}

void MainWindow::gpuDACT()
{
    QString dact;
    QStringList args = QString("PATH Win32_videocontroller get AdapterDACType").split(' ');
    command->start("wmic",args);
    command->waitForStarted();
    command->waitForFinished();
    dact = command->readAllStandardOutput();
    dact = dact.remove(0,dact.indexOf("\n")+1).remove(dact.length()-7,7);
    QStringList dac = dact.split('\n');
    for(int i = 0;i < dac.length();i++){
        dac[i] = dac[i].remove(0,dac[i].indexOf("\n")+1).remove(dac[i].indexOf("\r"),dac[i].length());
    }
    ui->lw_gpuType->addItems(dac);
}

void MainWindow::ramCapacity()
{
    QString capacity;
    double cap = 0;
    QStringList args = QString("MEMORYCHIP get Capacity").split(' ');
    command->start("wmic",args);
    command->waitForStarted();
    command->waitForFinished();
    capacity = command->readAllStandardOutput();
    capacity = capacity.remove(0,capacity.indexOf("\n")+1).remove(capacity.indexOf(" "),capacity.length());
    cap = capacity.toDouble();
    cap /= qPow(2,30);
    ui->le_capacity->setReadOnly(true);
    ui->le_capacity->setText(QString::number(cap)+"GB");
}

void MainWindow::ramSpeed()
{
    QString speed;
    QStringList args = QString("MEMORYCHIP get Speed").split(' ');
    command->start("wmic",args);
    command->waitForStarted();
    command->waitForFinished();
    speed = command->readAllStandardOutput();
    speed = speed.remove(0,speed.indexOf("\n")+1).remove(speed.indexOf(" "),speed.length());
    ui->le_speed->setReadOnly(true);
    ui->le_speed->setText(speed);
}

void MainWindow::ramManufecturer()
{
    QString manufacturer;
    QStringList args = QString("MEMORYCHIP get Manufacturer").split(' ');
    command->start("wmic",args);
    command->waitForStarted();
    command->waitForFinished();
    manufacturer = command->readAllStandardOutput();
    manufacturer = manufacturer.remove(0,manufacturer.indexOf("\n")+1).remove(manufacturer.indexOf("\r"),manufacturer.length());
    ui->le_manufacturer->setReadOnly(true);
    ui->le_manufacturer->setText(manufacturer);
}

void MainWindow::ramVoltage()
{
    QString voltage;
    QStringList args = QString("MEMORYCHIP get MaxVoltage").split(' ');
    command->start("wmic",args);
    command->waitForStarted();
    command->waitForFinished();
    voltage = command->readAllStandardOutput();
    voltage = voltage.remove(0,voltage.indexOf("\n")+1).remove(voltage.indexOf(" "),voltage.length());
    ui->le_voltage->setReadOnly(true);
    ui->le_voltage->setText(voltage);
}

void MainWindow::ramPostion()
{
    QString slot;
    QStringList args = QString("MEMORYCHIP get BankLabel").split(' ');
    command->start("wmic",args);
    command->waitForStarted();
    command->waitForFinished();
    slot = command->readAllStandardOutput();
    slot = slot.remove(0,slot.indexOf("\n")+1).remove(slot.indexOf("\r"),slot.length());
    ui->le_position->setReadOnly(true);
    ui->le_position->setText(slot);
}

void MainWindow::motherbordVersion()
{
    QString version;
    QStringList args = QString("baseboard get version").split(' ');
    command->start("wmic",args);
    command->waitForStarted();
    command->waitForFinished();
    version = command->readAllStandardOutput();
    version = version.remove(0,version.indexOf("\n")+1).remove(version.indexOf(" "),version.length());
    ui->le_basever->setReadOnly(true);
    ui->le_basever->setText(version);
}

void MainWindow::motherboardSerialnum()
{
    QString serial_num;
    QStringList args = QString("baseboard get serialnumber").split(' ');
    command->start("wmic",args);
    command->waitForStarted();
    command->waitForFinished();
    serial_num = command->readAllStandardOutput();
    serial_num = serial_num.remove(0,serial_num.indexOf("\n")+1).remove(serial_num.indexOf(" "),serial_num.length());
    ui->le_basesernum->setReadOnly(true);
    ui->le_basesernum->setText(serial_num);
}
