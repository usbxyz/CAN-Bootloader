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
}

MainWindow::~MainWindow()
{
    delete ui;
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
    bool state;
    ret = USB_ScanDevice(NULL);
    if(ret <= 0){
#ifdef LANGUE_EN
        QMessageBox::warning(this,"Warning","No device connected!");
#else
        QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("无设备连接！"));
#endif
        return false;
    }
    state = USB_OpenDevice(ui->deviceIndexComboBox->currentIndex());
    if(!state){
#ifdef LANGUE_EN
        QMessageBox::warning(this,"Warning","Open device faild!");
#else
        QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("打开设备失败！"));
#endif
        return false;
    }
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
    CAN_InitConfig.CAN_BRP = CANBaudRateTab[CAN_GetBaudRateNum(baud)].PreScale;
    CAN_InitConfig.CAN_SJW = CANBaudRateTab[CAN_GetBaudRateNum(baud)].SJW;
    CAN_InitConfig.CAN_BS1 = CANBaudRateTab[CAN_GetBaudRateNum(baud)].BS1;
    CAN_InitConfig.CAN_BS2 = CANBaudRateTab[CAN_GetBaudRateNum(baud)].BS2;

    ret = CAN_BL_Init(ui->deviceIndexComboBox->currentIndex(),
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
    bool ConfFlag;
    uint32_t appversion,appType;
    uint8_t FirmwareData[1026]={0};
    if(ui->allNodeCheckBox->isChecked()){
        if(ui->nodeListTableWidget->rowCount()<=0){
#ifdef LANGUE_EN
            QMessageBox::warning(this,"Warning","No CAN node!");
#else
            QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("无任何节点！"));
#endif
            return;
        }
    }else{
        if(ui->nodeListTableWidget->currentIndex().row()<0){
#ifdef LANGUE_EN
            QMessageBox::warning(this,"Warning","Please Select a CAN node!");
#else
            QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("请选择节点！"));
#endif
            return;
        }
    }
    uint16_t NodeAddr;
    ConfFlag = DeviceConfig();
    if(!ConfFlag){
        return;
    }
    if(ui->allNodeCheckBox->isChecked()){
        NodeAddr = 0x00;
        ret = CAN_BL_Excute(ui->deviceIndexComboBox->currentIndex(),
                            ui->channelIndexComboBox->currentIndex(),
                            NodeAddr,
                            CAN_BL_BOOT);
        if(ret != CAN_SUCCESS){
#ifdef LANGUE_EN
            QMessageBox::warning(this,"Warning","Execute firmware faild!");
#else
            QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("执行固件程序失败！"));
#endif
            USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
            return;
        }
        Sleep(500);
    }else{
        NodeAddr = ui->nodeListTableWidget->item(ui->nodeListTableWidget->currentIndex().row(),0)->text().toInt(NULL,16);
        ret = CAN_BL_NodeCheck(ui->deviceIndexComboBox->currentIndex(),
                            ui->channelIndexComboBox->currentIndex(),
                            NodeAddr,
                            &appversion,
                            &appType,
                            500);
        if(ret == CAN_SUCCESS){
            if(appType != CAN_BL_BOOT){//当前固件不为Bootloader
                ret = CAN_BL_Excute(ui->deviceIndexComboBox->currentIndex(),
                                    ui->channelIndexComboBox->currentIndex(),
                                    NodeAddr,
                                    CAN_BL_BOOT);
                if(ret != CAN_SUCCESS){
#ifdef LANGUE_EN
                    QMessageBox::warning(this,"Warning","Execute firmware faild!");
#else
                    QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("执行固件程序失败！"));
#endif
                    USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
                    return;
                }
                Sleep(500);
            }
        }else{
#ifdef LANGUE_EN
            QMessageBox::warning(this,"Warning","Check CAN node faild!");
#else
            QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("节点检测失败！"));
#endif
            USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
            return;
        }
    }
    QFile firmwareFile(ui->firmwareLineEdit->text());
    if (firmwareFile.open(QFile::ReadOnly)){
        if(!ui->allNodeCheckBox->isChecked()){
            ret = CAN_BL_NodeCheck(ui->deviceIndexComboBox->currentIndex(),
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
                    QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("当前固件不为Bootloader固件！"));
#endif
                    USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
                    return;
                }
            }else{
#ifdef LANGUE_EN
                QMessageBox::warning(this,"Warning","Check CAN node faild!");
#else
                QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("节点检测失败！"));
#endif
                USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
                return;
            }
        }
        ret = CAN_BL_Erase(ui->deviceIndexComboBox->currentIndex(),
                           ui->channelIndexComboBox->currentIndex(),
                           NodeAddr,
                           firmwareFile.size(),
                           1000);
        if(ret != CAN_SUCCESS){
            qDebug()<<"CBL_EraseFlash = "<<ret;
#ifdef LANGUE_EN
            QMessageBox::warning(this,"Warning","Erase flash faild!");
#else
            QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("擦出Flash失败！"));
#endif
            USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
            return;
        }
        if(ui->allNodeCheckBox->isChecked()){
            Sleep(1000);
        }
        int read_data_num;
#ifdef LANGUE_EN
        QProgressDialog writeDataProcess("Upgrade firmware...","Cancel",0,firmwareFile.size(),this);
        writeDataProcess.setWindowTitle("Upgrade firmware");
#else
        QProgressDialog writeDataProcess(QStringLiteral("正在更新固件..."),QStringLiteral("取消"),0,firmwareFile.size(),this);
        writeDataProcess.setWindowTitle(QStringLiteral("更新固件"));
#endif
        writeDataProcess.setModal(true);
        writeDataProcess.show();
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        int i=0;
        int PackSize = 512;
        for(i=0;i<firmwareFile.size();i+=PackSize){
            read_data_num = firmwareFile.read((char*)FirmwareData,PackSize);
            ret = CAN_BL_Write(ui->deviceIndexComboBox->currentIndex(),
                               ui->channelIndexComboBox->currentIndex(),
                               NodeAddr,
                               i,
                               FirmwareData,
                               read_data_num,
                               1000);
            if(ret != CAN_SUCCESS){
#ifdef LANGUE_EN
                QMessageBox::warning(this,"Warning","Write flash faild!");
#else
                QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("写Flash数据失败！"));
#endif
                USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
                return;
            }
            writeDataProcess.setValue(i);
            QCoreApplication::processEvents(QEventLoop::AllEvents);
            if(writeDataProcess.wasCanceled()){
                USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
                return;
            }
            if(ui->allNodeCheckBox->isChecked()){
                Sleep(10);
            }
        }
        writeDataProcess.setValue(firmwareFile.size());
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        if(writeDataProcess.wasCanceled()){
            USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
            return;
        }
    }else{
        USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
#ifdef LANGUE_EN
        QMessageBox::warning(this,"Warning","Open firmware file faild!");
#else
        QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("打开固件文件失败！"));
#endif
        return;
    }
    //执行固件
    ret = CAN_BL_Excute(ui->deviceIndexComboBox->currentIndex(),
                        ui->channelIndexComboBox->currentIndex(),
                        NodeAddr,
                        CAN_BL_APP);
    if(ret != CAN_SUCCESS){
#ifdef LANGUE_EN
        QMessageBox::warning(this,"Warning","Execute firmware faild!");
#else
        QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("执行固件程序失败！"));
#endif
    }
    Sleep(50);
    if(!ui->allNodeCheckBox->isChecked()){
        ret = CAN_BL_NodeCheck(ui->deviceIndexComboBox->currentIndex(),
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
            QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("执行固件程序失败！"));
        }
    }
    USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
    qDebug()<<time.elapsed()/1000.0<<"s";
}

void MainWindow::on_openFirmwareFileAction_triggered()
{
    on_openFirmwareFilePushButton_clicked();
}

void MainWindow::on_scanNodeAction_triggered()
{
    int ret;
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
        USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
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
    QProgressDialog scanNodeProcess(QStringLiteral("正在扫描节点..."),QStringLiteral("取消"),0,endAddr-startAddr,this);
    scanNodeProcess.setWindowTitle(QStringLiteral("扫描节点"));
#endif
    scanNodeProcess.setModal(true);
    scanNodeProcess.show();
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    int i=0;
    while(startAddr <= endAddr){
        uint32_t appversion,appType;
        i++;
        ret = CAN_BL_NodeCheck(ui->deviceIndexComboBox->currentIndex(),
                            ui->channelIndexComboBox->currentIndex(),
                            startAddr,
                            &appversion,
                            &appType,
                            10);
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
        scanNodeProcess.setValue(i);
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        if(scanNodeProcess.wasCanceled()){
            USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
            return;
        }
        startAddr++;
    }
    USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
}



void MainWindow::on_setbaudRatePushButton_clicked()
{
    int ret;
    CAN_INIT_CONFIG CAN_InitConfig;
    if(ui->allNodeCheckBox->isChecked()){
        if(ui->nodeListTableWidget->rowCount()<=0){
#ifdef LANGUE_EN
            QMessageBox::warning(this,"Warning","No CAN node!");
#else
            QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("无任何节点！"));
#endif
            return;
        }
    }else{
        if(ui->nodeListTableWidget->currentIndex().row()<0){
#ifdef LANGUE_EN
            QMessageBox::warning(this,"Warning","Please select a CAN node!");
#else
            QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("请选择节点！"));
#endif
            return;
        }
    }
    bool ConfFlag = DeviceConfig();
    if(!ConfFlag){
        USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
        return;
    }
    QString str = ui->newBaudRateComboBox->currentText();
    str.resize(str.length()-4);
    int baud = str.toInt(NULL,10)*1000;
    CAN_InitConfig.CAN_BRP = CANBaudRateTab[CAN_GetBaudRateNum(baud)].PreScale;
    CAN_InitConfig.CAN_SJW = CANBaudRateTab[CAN_GetBaudRateNum(baud)].SJW;
    CAN_InitConfig.CAN_BS1 = CANBaudRateTab[CAN_GetBaudRateNum(baud)].BS1;
    CAN_InitConfig.CAN_BS2 = CANBaudRateTab[CAN_GetBaudRateNum(baud)].BS2;
    uint16_t NodeAddr;
    if(ui->allNodeCheckBox->isChecked()){
        NodeAddr = 0x00;
    }else{
        NodeAddr = ui->nodeListTableWidget->item(ui->nodeListTableWidget->currentIndex().row(),0)->text().toInt(NULL,16);
    }
    ret = CAN_BL_SetNewBaudRate(ui->deviceIndexComboBox->currentIndex(),
                                ui->channelIndexComboBox->currentIndex(),
                                NodeAddr,
                                &CAN_InitConfig,
                                baud,
                                100);
    if(ret != CAN_SUCCESS){
        USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
#ifdef LANGUE_EN
        QMessageBox::warning(this,"Warning","Set baud rate faild!");
#else
        QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("设置波特率失败！"));
#endif
        return;
    }
    ui->baudRateComboBox->setCurrentIndex(ui->newBaudRateComboBox->currentIndex());
    USB_CloseDevice(ui->deviceIndexComboBox->currentIndex());
}

void MainWindow::on_contactUsAction_triggered()
{
    QString AboutStr;
#ifndef LANG_EN
    AboutStr.append(("官方网站<span style=\"font-size:12px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>：<a href=\"http://www.usbxyz.com\">www.usbxyz.com</a><br>"));
    AboutStr.append(("官方论坛<span style=\"font-size:12px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>：<a href=\"http://www.embed-net.com\">www.embed-net.com</a><br>"));
    AboutStr.append(("官方淘宝店<span style=\"font-size:9px;\">&nbsp;&nbsp;&nbsp;</span>：<a href=\"http://usbxyz.taobao.com/\">usbxyz.taobao.com</a><br>"));
    AboutStr.append(("技术支持QQ：188298598<br>"));
    AboutStr.append(("产品咨询QQ：188298598"));
    QMessageBox::about(this,("联系我们"),AboutStr);
#else
    AboutStr.append("Official website<span style=\"font-size:12px;\">&nbsp;</span>:&nbsp;<a href=\"http://www.usbxyz.com\">www.usbxyz.com</a><br>");
    AboutStr.append("Official BBS<span style=\"font-size:12px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>:&nbsp;<a href=\"http://www.embed-net.com\">www.embed-net.com</a><br>");
    AboutStr.append("Official Taobao<span style=\"font-size: 9px;\">&nbsp;</span>:&nbsp;<a href=\"http://usbxyz.taobao.com/\">usbxyz.taobao.com</a><br>");
    AboutStr.append("Technical Suuport QQ&nbsp;:&nbsp;188298598<br>");
    AboutStr.append("Product Consultant QQ&nbsp;:&nbsp;188298598");
    QMessageBox::about(this,"Contact Us",AboutStr);
#endif
}

void MainWindow::on_aboutAction_triggered()
{
    QString AboutStr;
    AboutStr.append("USB2XXX USB2CAN Bootloader 1.0.1<br>");
#ifndef LANG_EN
    AboutStr.append(("支持硬件：USB2XXX<br>"));
    AboutStr.append(("购买地址：<a href=\"http://usbxyz.taobao.com/\">usbxyz.taobao.com</a>"));
    QMessageBox::about(this,("关于USB2XXX USB2CAN Bootloader"),AboutStr);
#else
    AboutStr.append("Hardware Suuport: USB2XXX<br>");
    AboutStr.append(("Purchase URL:<a href=\"http://usbxyz.taobao.com/\">usbxyz.taobao.com</a>"));
    QMessageBox::about(this,"About USB2XXX USB2CAN Bootloader",AboutStr);
#endif
}

void MainWindow::on_exitAction_triggered()
{
    this->close();
}


