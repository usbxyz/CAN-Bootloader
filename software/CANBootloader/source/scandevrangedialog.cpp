#include "scandevrangedialog.h"
#include "ui_scandevrangedialog.h"

ScanDevRangeDialog::ScanDevRangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScanDevRangeDialog)
{
    ui->setupUi(this);
}

ScanDevRangeDialog::~ScanDevRangeDialog()
{
    delete ui;
}

void ScanDevRangeDialog::on_spinBoxStartAddr_valueChanged(int arg1)
{
    if(arg1 > ui->spinBoxEndAddr->value()){
        ui->spinBoxEndAddr->setValue(arg1);
    }
}

void ScanDevRangeDialog::on_spinBoxEndAddr_valueChanged(int arg1)
{
    if(arg1 < ui->spinBoxStartAddr->value()){
        ui->spinBoxStartAddr->setValue(arg1);
    }
}

void ScanDevRangeDialog::on_pushButtonConfirm_clicked()
{
    StartAddr = ui->spinBoxStartAddr->value();
    EndAddr = ui->spinBoxEndAddr->value();
    this->accept();
}

void ScanDevRangeDialog::on_pushButtonCancel_clicked()
{
    this->reject();
}
