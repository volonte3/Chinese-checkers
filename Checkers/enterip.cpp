#include "enterip.h"
#include "ui_enterip.h"

EnterIP::EnterIP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnterIP)
{
    ui->setupUi(this);
}

EnterIP::~EnterIP()
{
    delete ui;
}
