#include "windialog.h"
#include "ui_windialog.h"
#include <QDebug>

WinDialog::WinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WinDialog)
{
    ui->setupUi(this);
}

WinDialog::~WinDialog()
{
    delete ui;
}
