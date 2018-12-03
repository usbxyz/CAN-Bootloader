#include "dialogaddnode.h"
#include "ui_dialogaddnode.h"
#include "usb2can.h"
DialogAddNode::DialogAddNode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddNode)
{
    ui->setupUi(this);
    NodeAddr = 1;
}

DialogAddNode::~DialogAddNode()
{
    delete ui;
}

void DialogAddNode::on_pushButtonOK_clicked()
{
    NodeAddr = ui->spinBoxNodeAddr->value();
    NodeType = ui->comboBoxNodeType->currentText()=="BOOT"?CAN_BL_BOOT:CAN_BL_APP;
    this->accept();
}

void DialogAddNode::on_pushButtonCancel_clicked()
{
    this->reject();
}
