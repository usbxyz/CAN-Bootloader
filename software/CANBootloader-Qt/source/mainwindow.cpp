#include "mainwindow.h"
#ifdef LANGUE_EN
#include "ui_mainwindow_en.h"
#else
#include "ui_mainwindow_ch.h"
#endif
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->cmdListTableWidget->setColumnWidth(0,180);
#ifdef LANGUE_EN
    ui->nodeListTableWidget->setColumnWidth(0,100);
    ui->nodeListTableWidget->setColumnWidth(1,100);
    ui->nodeListTableWidget->setColumnWidth(2,150);
#endif
    for(int i=0;i<ui->cmdListTableWidget->rowCount();i++){
        ui->cmdListTableWidget->setRowHeight(i,20);
    }
    //qDebug << "sizeof(CANBaudRateTab) = " << sizeof(CANBaudRateTab);
    ScanDevice();
    DevTypeUSB2CANB = ((uint64_t)'U'<<56)|((uint64_t)'S'<<48)|((uint64_t)'B'<<40)|((uint64_t)'2'<<32)|((uint64_t)'C'<<24)|((uint64_t)'A'<<16)|((uint64_t)'N'<<8)|((uint64_t)'B'<<0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ScanDevice(void)
{
    int DevHandle[20];
    int deviceNum = USB_ScanDevice(DevHandle);
    ui->deviceHandleComboBox->clear();
    for(int i=0;i<deviceNum;i++){
        ui->deviceHandleComboBox->addItem(QString().sprintf("%08X",DevHandle[i]));
    }
}


void MainWindow::on_openFirmwareFilePushButton_clicked()
{
    QString fileName;
    fileName=QFileDialog::getOpenFileName(this,
                                          tr("Open files"),
                                          "",
                                          "Binary Files (*.bin);;Hex Files (*.hex);;All Files (*.*)");
    if(fileName.isNull()){
        return;
    }
    ui->firmwareLineEdit->setText(fileName);
}
int MainWindow::CAN_GetBaudRateNum(unsigned int BaudRate)
{
    for(int i=0;i<27;i++){
        if(BaudRate == CANBaudRateTab[i].BaudRate){
            return i;
        }
    }
    return 0;
}
bool MainWindow::DeviceConfig(void)
{
    int ret;
    int DeviceHandle = ui->deviceHandleComboBox->currentText().toInt(NULL,16);
    bool state;
    state = USB_OpenDevice(DeviceHandle);
    if(!state){
#ifdef LANGUE_EN
        QMessageBox::warning(this,"Warning","Open device faild!");
#else
        QMessageBox::warning(this,"警告","打开设备失败！");
#endif
        return false;
    }
    DEVICE_INFO DevInfo;
    char FuncStr[256];
    state = DEV_GetDeviceInfo(DeviceHandle,&DevInfo,FuncStr);
    if(!state){
#ifdef LANGUE_EN
        QMessageBox::warning(this,"Warning","Get device infomation faild!");
#else
        QMessageBox::warning(this,"警告","获取设备信息失败！");
#endif
        return false;
    }
    DevType = ((uint64_t)DevInfo.SerialNumber[0]<<32)|(DevInfo.SerialNumber[1]);
    CBL_CMD_LIST CMD_List;
    QString cmdStr[]={"Erase","WriteInfo","Write","Check","SetBaudRate","Excute","CmdSuccess","CmdFaild"};
    uint8_t cmdData[16];
    for(int i=0;i<ui->cmdListTableWidget->rowCount();i++){
        if(ui->cmdListTableWidget->item(i,0)->text()==cmdStr[i]){
            cmdData[i] = ui->cmdListTableWidget->item(i,1)->text().toInt(NULL,16);
        }
    }
    CMD_List.Erase = cmdData[0];
    CMD_List.WriteInfo = cmdData[1];
    CMD_List.Write = cmdData[2];
    CMD_List.Check = cmdData[3];
    CMD_List.SetBaudRate = cmdData[4];
    CMD_List.Excute = cmdData[5];
    CMD_List.CmdSuccess = cmdData[6];
    CMD_List.CmdFaild = cmdData[7];
    CAN_INIT_CONFIG CAN_InitConfig;
    QString str = ui->baudRateComboBox->currentText();
    str.resize(str.length()-4);
    int baud = str.toInt(NULL,10)*1000;
    qDebug()<<"baud = "<<baud;
    if(DevType == DevTypeUSB2CANB){
        qDebug()<<"DevType = USB2CANB";
        CAN_InitConfig.CAN_BRP = CANBaudRateTab42M[CAN_GetBaudRateNum(baud)].PreScale;
        CAN_InitConfig.CAN_SJW = CANBaudRateTab42M[CAN_GetBaudRateNum(baud)].SJW;
        CAN_InitConfig.CAN_BS1 = CANBaudRateTab42M[CAN_GetBaudRateNum(baud)].BS1;
        CAN_InitConfig.CAN_BS2 = CANBaudRateTab42M[CAN_GetBaudRateNum(baud)].BS2;
    }else{
        qDebug()<<"DevType = USB2XXXB";
        CAN_InitConfig.CAN_BRP = CANBaudRateTab[CAN_GetBaudRateNum(baud)].PreScale;
        CAN_InitConfig.CAN_SJW = CANBaudRateTab[CAN_GetBaudRateNum(baud)].SJW;
        CAN_InitConfig.CAN_BS1 = CANBaudRateTab[CAN_GetBaudRateNum(baud)].BS1;
        CAN_InitConfig.CAN_BS2 = CANBaudRateTab[CAN_GetBaudRateNum(baud)].BS2;
    }
    if(ui->checkBoxEnResistor->isChecked()){
        CAN_InitConfig.CAN_Mode |= 0x80;
    }
    ret = CAN_BL_Init(DeviceHandle,
                     ui->channelIndexComboBox->currentIndex(),
                     &CAN_InitConfig,
                     &CMD_List);
    if(ret!=CAN_SUCCESS){
#ifdef LANGUE_EN
        QMessageBox::warning(this,"Warning","Config device faild!");
#else
        QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("配置设备失败！"));
#endif
        return false;
    }
    return true;
}
void MainWindow::on_updateFirmwarePushButton_clicked()
{
    QTime time;
    time.start();
    int ret;
    int DeviceHandle = ui->deviceHandleComboBox->currentText().toInt(NULL,16);
    bool ConfFlag;
    uint32_t appversion,appType;
    uint8_t FirmwareData[1026]={0};
    if(ui->allNodeCheckBox->isChecked()){
        if(ui->nodeListTableWidget->rowCount()<=0){
#ifdef LANGUE_EN
            QMessageBox::warning(this,"Warning","No CAN node!");
#else
            QMessageBox::warning(this,"警告","无任何节点！");
#endif
            return;
        }
    }else{
        if(ui->nodeListTableWidget->currentIndex().row()<0){
#ifdef LANGUE_EN
            QMessageBox::warning(this,"Warning","Please Select a CAN node!");
#else
            QMessageBox::warning(this,"警告","请选择节点！");
#endif
            return;
        }
    }
    uint16_t NodeAddr=0;
    ConfFlag = DeviceConfig();
    if(!ConfFlag){
        return;
    }
    if(ui->allNodeCheckBox->isChecked()){
        NodeAddr = 0x00;
        ret = CAN_BL_Excute(DeviceHandle,
                            ui->channelIndexComboBox->currentIndex(),
                            NodeAddr,
                            CAN_BL_BOOT);
        if(ret != CAN_SUCCESS){
#ifdef LANGUE_EN
            QMessageBox::warning(this,"Warning","Execute firmware faild!");
#else
            QMessageBox::warning(this,"警告","执行固件程序失败！");
#endif
            USB_CloseDevice(DeviceHandle);
            return;
        }
#ifndef OS_UNIX
        Sleep(500);
#else
        usleep(500*1000);
#endif
    }else{
        NodeAddr = ui->nodeListTableWidget->item(ui->nodeListTableWidget->currentIndex().row(),0)->text().toInt(NULL,16);
        ret = CAN_BL_NodeCheck(DeviceHandle,
                            ui->channelIndexComboBox->currentIndex(),
                            NodeAddr,
                            &appversion,
                            &appType,
                            500);
        if(ret == CAN_SUCCESS){
            if(appType != CAN_BL_BOOT){//当前固件不为Bootloader
                ret = CAN_BL_Excute(DeviceHandle,
                                    ui->channelIndexComboBox->currentIndex(),
                                    NodeAddr,
                                    CAN_BL_BOOT);
                if(ret != CAN_SUCCESS){
#ifdef LANGUE_EN
                    QMessageBox::warning(this,"Warning","Execute firmware faild!");
#else
                    QMessageBox::warning(this,"警告","执行固件程序失败！");
#endif
                    USB_CloseDevice(DeviceHandle);
                    return;
                }
#ifndef OS_UNIX
                Sleep(500);
#else
                usleep(500*1000);
#endif
            }
        }else{
#ifdef LANGUE_EN
            QMessageBox::warning(this,"Warning","Check CAN node faild!");
#else
            QMessageBox::warning(this,"警告","节点检测失败！");
#endif
            USB_CloseDevice(DeviceHandle);
            return;
        }
    }
    QFile firmwareFile(ui->firmwareLineEdit->text());
    if (firmwareFile.open(QFile::ReadOnly)){
        if(!ui->allNodeCheckBox->isChecked()){
            ret = CAN_BL_NodeCheck(DeviceHandle,
                                ui->channelIndexComboBox->currentIndex(),
                                NodeAddr,
                                &appversion,
                                &appType,
                                100);
            if(ret == CAN_SUCCESS){
                if(appType != CAN_BL_BOOT){//当前固件不为Bootloader
#ifdef LANGUE_EN
                    QMessageBox::warning(this,"Warning","Current firmware is not bootloader!");
#else
                    QMessageBox::warning(this,"警告","当前固件不为Bootloader固件！");
#endif
                    USB_CloseDevice(DeviceHandle);
                    return;
                }
            }else{
#ifdef LANGUE_EN
                QMessageBox::warning(this,"Warning","Check CAN node faild!");
#else
                QMessageBox::warning(this,"警告","节点检测失败！");
#endif
                USB_CloseDevice(DeviceHandle);
                return;
            }
        }
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        ret = CAN_BL_Erase(DeviceHandle,
                           ui->channelIndexComboBox->currentIndex(),
                           NodeAddr,
                           firmwareFile.size(),
                           8000);
        if(ret != CAN_SUCCESS){
            qDebug()<<"CBL_EraseFlash = "<<ret;
#ifdef LANGUE_EN
            QMessageBox::warning(this,"Warning","Erase flash faild!");
#else
            QMessageBox::warning(this,"警告",QString().sprintf("擦出Flash失败！%d",ret));
#endif
            USB_CloseDevice(DeviceHandle);
            return;
        }
        //批量更新的时候，由于不检测节点状态返回，所以需要在这里增加延时函数，延时的时间一定要大于芯片数据擦除的时间，否则后面写入数据会失败
        if(ui->allNodeCheckBox->isChecked()){
#ifndef OS_UNIX
            Sleep(1000);
#else
            usleep(1000*1000);
#endif
        }
        int read_data_num;
#ifdef LANGUE_EN
        QProgressDialog writeDataProcess("Upgrade firmware...","Cancel",0,firmwareFile.size(),this);
        writeDataProcess.setWindowTitle("Upgrade firmware");
#else
        QProgressDialog writeDataProcess("正在更新固件...","取消",0,firmwareFile.size(),this);
        writeDataProcess.setWindowTitle("更新固件");
#endif
        writeDataProcess.setModal(true);
        writeDataProcess.show();
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        int i=0;
        int PackSize = 256;
        for(i=0;i<firmwareFile.size();i+=PackSize){
            read_data_num = firmwareFile.read((char*)FirmwareData,PackSize);
            ret = CAN_BL_Write(DeviceHandle,
                               ui->channelIndexComboBox->currentIndex(),
                               NodeAddr,
                               i,
                               FirmwareData,
                               read_data_num,
                               3000);
            if(ret != CAN_SUCCESS){
#ifdef LANGUE_EN
                QMessageBox::warning(this,"Warning","Write flash faild!");
#else
                QMessageBox::warning(this,"警告",QString().sprintf("写Flash数据失败！%d",ret));
#endif
                USB_CloseDevice(DeviceHandle);
                return;
            }
            writeDataProcess.setValue(i);
            QCoreApplication::processEvents(QEventLoop::AllEvents);
            if(writeDataProcess.wasCanceled()){
                USB_CloseDevice(DeviceHandle);
                return;
            }
            if(ui->allNodeCheckBox->isChecked()){//批量更新的时候，由于不检测节点状态返回，所以需要在这里增加延时函数
#ifndef OS_UNIX
                Sleep(30);
#else
                usleep(30*1000);
#endif
            }
        }
        writeDataProcess.setValue(firmwareFile.size());
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        if(writeDataProcess.wasCanceled()){
            USB_CloseDevice(DeviceHandle);
            return;
        }
    }else{
        USB_CloseDevice(DeviceHandle);
#ifdef LANGUE_EN
        QMessageBox::warning(this,"Warning","Open firmware file faild!");
#else
        QMessageBox::warning(this,"警告","打开固件文件失败！");
#endif
        return;
    }
    //执行固件
    ret = CAN_BL_Excute(DeviceHandle,
                        ui->channelIndexComboBox->currentIndex(),
                        NodeAddr,
                        CAN_BL_APP);
    if(ret != CAN_SUCCESS){
#ifdef LANGUE_EN
        QMessageBox::warning(this,"Warning","Execute firmware faild!");
#else
        QMessageBox::warning(this,"警告",QString().sprintf("执行固件程序失败！%d",ret));
#endif
    }
#ifndef OS_UNIX
    Sleep(50);
#else
    usleep(50*1000);
#endif
    if(!ui->allNodeCheckBox->isChecked()){
        ret = CAN_BL_NodeCheck(DeviceHandle,
                            ui->channelIndexComboBox->currentIndex(),
                            NodeAddr,
                            &appversion,
                            &appType,
                            500);
        if(ret == CAN_SUCCESS){
            QString str;
            if(appType == CAN_BL_BOOT){
                str = "BOOT";
            }else{
                str = "APP";
            }
            ui->nodeListTableWidget->item(ui->nodeListTableWidget->currentIndex().row(),1)->setText(str);
            str.sprintf("v%d.%d",(((appversion>>24)&0xFF)*10)+(appversion>>16)&0xFF,(((appversion>>8)&0xFF)*10)+appversion&0xFF);
            ui->nodeListTableWidget->item(ui->nodeListTableWidget->currentIndex().row(),2)->setText(str);
        }else{
            QMessageBox::warning(this,"警告",QString().sprintf("检测固件程序执行失败！%d",ret));
        }
    }
    USB_CloseDevice(DeviceHandle);
    qDebug()<<time.elapsed()/1000.0<<"s";
}

void MainWindow::on_openFirmwareFileAction_triggered()
{
    on_openFirmwareFilePushButton_clicked();
}

void MainWindow::on_scanNodeAction_triggered()
{
    int ret;
    int DeviceHandle = ui->deviceHandleComboBox->currentText().toInt(NULL,16);
    int startAddr = 0,endAddr = 0;
    ScanDevRangeDialog *pScanDevRangeDialog = new ScanDevRangeDialog();
    if(pScanDevRangeDialog->exec() == QDialog::Accepted){
        startAddr = pScanDevRangeDialog->StartAddr;
        endAddr = pScanDevRangeDialog->EndAddr;
    }else{
        return ;
    }
    bool ConfFlag = DeviceConfig();
    if(!ConfFlag){
        USB_CloseDevice(DeviceHandle);
        return;
    }
    ui->nodeListTableWidget->verticalHeader()->hide();
    ui->nodeListTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->nodeListTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->nodeListTableWidget->setRowCount(0);
#ifdef LANGUE_EN
    QProgressDialog scanNodeProcess("Scanning CAN node...","Cancel",0,MAX_NODE_NUM,this);
    scanNodeProcess.setWindowTitle("Scanning CAN node");
#else
    QProgressDialog scanNodeProcess("正在扫描节点...","取消",0,endAddr-startAddr,this);
    scanNodeProcess.setWindowTitle("扫描节点");
#endif
    scanNodeProcess.setModal(true);
    scanNodeProcess.show();
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    int i=0;
    while(startAddr <= endAddr){
        uint32_t appversion,appType;
        i++;
        ret = CAN_BL_NodeCheck(DeviceHandle,
                            ui->channelIndexComboBox->currentIndex(),
                            startAddr,
                            &appversion,
                            &appType,
                            100);
        if(ret == CAN_SUCCESS){
            ui->nodeListTableWidget->setRowCount(ui->nodeListTableWidget->rowCount()+1);
            ui->nodeListTableWidget->setRowHeight(ui->nodeListTableWidget->rowCount()-1,20);
            QString str;
            str.sprintf("0x%X",startAddr);
            QTableWidgetItem *item = new QTableWidgetItem(str);
            ui->nodeListTableWidget->setItem(ui->nodeListTableWidget->rowCount()-1,0,item);
            if(appType == CAN_BL_BOOT){
                str = "BOOT";
            }else{
                str = "APP";
            }
            item = new QTableWidgetItem(str);
            ui->nodeListTableWidget->setItem(ui->nodeListTableWidget->rowCount()-1,1,item);
            str.sprintf("v%d.%d",(((appversion>>24)&0xFF)*10)+(appversion>>16)&0xFF,(((appversion>>8)&0xFF)*10)+appversion&0xFF);
            item = new QTableWidgetItem(str);
            ui->nodeListTableWidget->setItem(ui->nodeListTableWidget->rowCount()-1,2,item);
        }
        //qDebug()<<"ret = "<<ret;
        scanNodeProcess.setValue(i);
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        if(scanNodeProcess.wasCanceled()){
            USB_CloseDevice(DeviceHandle);
            return;
        }
        startAddr++;
    }
    USB_CloseDevice(DeviceHandle);
}



void MainWindow::on_setbaudRatePushButton_clicked()
{
    int ret;
    int DeviceHandle = ui->deviceHandleComboBox->currentText().toInt(NULL,16);
    CAN_INIT_CONFIG CAN_InitConfig;
    if(ui->allNodeCheckBox->isChecked()){
        if(ui->nodeListTableWidget->rowCount()<=0){
#ifdef LANGUE_EN
            QMessageBox::warning(this,"Warning","No CAN node!");
#else
            QMessageBox::warning(this,"警告","无任何节点！");
#endif
            return;
        }
    }else{
        if(ui->nodeListTableWidget->currentIndex().row()<0){
#ifdef LANGUE_EN
            QMessageBox::warning(this,"Warning","Please select a CAN node!");
#else
            QMessageBox::warning(this,"警告","请选择节点！");
#endif
            return;
        }
    }
    bool ConfFlag = DeviceConfig();
    if(!ConfFlag){
        USB_CloseDevice(DeviceHandle);
        return;
    }
    QString str = ui->newBaudRateComboBox->currentText();
    str.resize(str.length()-4);
    int baud = str.toInt(NULL,10)*1000;
    qDebug()<<"baud = "<<baud;
    if(DevType == DevTypeUSB2CANB){
        qDebug()<<"DevType = USB2CANB";
        CAN_InitConfig.CAN_BRP = CANBaudRateTab42M[CAN_GetBaudRateNum(baud)].PreScale;
        CAN_InitConfig.CAN_SJW = CANBaudRateTab42M[CAN_GetBaudRateNum(baud)].SJW;
        CAN_InitConfig.CAN_BS1 = CANBaudRateTab42M[CAN_GetBaudRateNum(baud)].BS1;
        CAN_InitConfig.CAN_BS2 = CANBaudRateTab42M[CAN_GetBaudRateNum(baud)].BS2;
    }else{
        qDebug()<<"DevType = USB2XXXB";
        CAN_InitConfig.CAN_BRP = CANBaudRateTab[CAN_GetBaudRateNum(baud)].PreScale;
        CAN_InitConfig.CAN_SJW = CANBaudRateTab[CAN_GetBaudRateNum(baud)].SJW;
        CAN_InitConfig.CAN_BS1 = CANBaudRateTab[CAN_GetBaudRateNum(baud)].BS1;
        CAN_InitConfig.CAN_BS2 = CANBaudRateTab[CAN_GetBaudRateNum(baud)].BS2;
    }
    if(ui->checkBoxEnResistor->isChecked()){
        CAN_InitConfig.CAN_Mode |= 0x80;
    }
    uint16_t NodeAddr;
    if(ui->allNodeCheckBox->isChecked()){
        NodeAddr = 0x00;
    }else{
        NodeAddr = ui->nodeListTableWidget->item(ui->nodeListTableWidget->currentIndex().row(),0)->text().toInt(NULL,16);
    }
    ret = CAN_BL_SetNewBaudRate(DeviceHandle,
                                ui->channelIndexComboBox->currentIndex(),
                                NodeAddr,
                                &CAN_InitConfig,
                                baud,
                                100);
    if(ret != CAN_SUCCESS){
        USB_CloseDevice(DeviceHandle);
#ifdef LANGUE_EN
        QMessageBox::warning(this,"Warning","Set baud rate faild!");
#else
        QMessageBox::warning(this,"警告","设置波特率失败！");
#endif
        return;
    }
    ui->baudRateComboBox->setCurrentIndex(ui->newBaudRateComboBox->currentIndex());
    USB_CloseDevice(DeviceHandle);
}

void MainWindow::on_contactUsAction_triggered()
{
    return;
    QString AboutStr;
#ifndef LANG_EN
    AboutStr.append(("官方网站<span style=\"font-size:12px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>：<a href=\"http://www.usbxyz.com\">www.usbxyz.com</a><br>"));
    AboutStr.append(("官方论坛<span style=\"font-size:12px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>：<a href=\"http://www.embed-net.com\">www.embed-net.com</a><br>"));
    AboutStr.append(("官方淘宝店<span style=\"font-size:9px;\">&nbsp;&nbsp;&nbsp;</span>：<a href=\"http://usb2xxx.taobao.com/\">usb2xxx.taobao.com</a><br>"));
    AboutStr.append(("技术支持QQ：188298598<br>"));
    AboutStr.append(("产品咨询QQ：188298598"));
    QMessageBox::about(this,("联系我们"),AboutStr);
#else
    AboutStr.append("Official website<span style=\"font-size:12px;\">&nbsp;</span>:&nbsp;<a href=\"http://www.usbxyz.com\">www.usbxyz.com</a><br>");
    AboutStr.append("Official BBS<span style=\"font-size:12px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>:&nbsp;<a href=\"http://www.embed-net.com\">www.embed-net.com</a><br>");
    AboutStr.append("Official Taobao<span style=\"font-size: 9px;\">&nbsp;</span>:&nbsp;<a href=\"http://usb2xxx.taobao.com/\">usb2xxx.taobao.com</a><br>");
    AboutStr.append("Technical Suuport QQ&nbsp;:&nbsp;188298598<br>");
    AboutStr.append("Product Consultant QQ&nbsp;:&nbsp;188298598");
    QMessageBox::about(this,"Contact Us",AboutStr);
#endif
}

void MainWindow::on_aboutAction_triggered()
{
    return;
    QString AboutStr;
    AboutStr.append("USB2XXX USB2CAN Bootloader 1.0.3<br>");
#ifndef LANG_EN
    AboutStr.append(("支持硬件：USB2XXX<br>"));
    AboutStr.append(("购买地址：<a href=\"http://usb2xxx.taobao.com/\">usb2xxx.taobao.com</a>"));
    QMessageBox::about(this,("关于USB2XXX USB2CAN Bootloader"),AboutStr);
#else
    AboutStr.append("Hardware Suuport: USB2XXX<br>");
    AboutStr.append(("Purchase URL:<a href=\"http://usb2xxx.taobao.com/\">usb2xxx.taobao.com</a>"));
    QMessageBox::about(this,"About USB2XXX USB2CAN Bootloader",AboutStr);
#endif
}

void MainWindow::on_exitAction_triggered()
{
    this->close();
}



void MainWindow::on_actionAddNode_triggered()
{
    DialogAddNode *pDialogAddNode = new DialogAddNode(this);
    if(pDialogAddNode->exec() == QDialog::Accepted){
        ui->nodeListTableWidget->setRowCount(ui->nodeListTableWidget->rowCount()+1);
        ui->nodeListTableWidget->setRowHeight(ui->nodeListTableWidget->rowCount()-1,20);
        QString str;
        str.sprintf("0x%X",pDialogAddNode->NodeAddr);
        QTableWidgetItem *item = new QTableWidgetItem(str);
        ui->nodeListTableWidget->setItem(ui->nodeListTableWidget->rowCount()-1,0,item);
        if(pDialogAddNode->NodeType == CAN_BL_BOOT){
            str = "BOOT";
        }else{
            str = "APP";
        }
        item = new QTableWidgetItem(str);
        ui->nodeListTableWidget->setItem(ui->nodeListTableWidget->rowCount()-1,1,item);
        //str.sprintf("v%d.%d",(((appversion>>24)&0xFF)*10)+(appversion>>16)&0xFF,(((appversion>>8)&0xFF)*10)+appversion&0xFF);
        //item = new QTableWidgetItem(str);
        //ui->nodeListTableWidget->setItem(ui->nodeListTableWidget->rowCount()-1,2,item);
    }
}
