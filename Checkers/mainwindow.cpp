#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include <QIcon>
#include <QPainter>
#include <QTimer>
#include <QLineEdit>
#include <QInputDialog>
#include <QValidator>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QMessageBox>
#define cout qDebug()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->action3->setEnabled(false);
    this->sts = NULL;
    this->sts1 =NULL;
    initscene();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/firstmap.png");
    painter.drawPixmap(0,0,pix);
}

void MainWindow::initscene()
{
    setWindowTitle(GAME_TITLE);
    setWindowIcon(QIcon(":/res/game.ico"));
    setFixedSize(FIRST_WIDTH,FIRST_HEIGHT);
}

void MainWindow::on_action3_triggered()
{
    this->sts->getready();
    if(this->sts->hready){
        this->hide();
        this->sts->initscene1();
        this->sts->setGeometry(this->geometry());
        this->sts->show();
        this->sts->cdtimer->start();
    }
    connect(this->sts,&NetGame::okready,this,[=](){
        this->hide();
        this->sts->initscene1();
        this->sts->setGeometry(this->geometry());
        this->sts->show();
        this->sts->cdtimer->start();
    });
}


void MainWindow::on_action_triggered()
{
    this->sts = new NetGame(true);
    connect(this->sts,&NetGame::StartSceneBack,this,[=](){
        this->sts->hide();
        this->show();
        this->sts->cdtimer->stop();
        this->sts->cdtimereset();
    });
    connect(this->sts,&NetGame::cancli,this,[=](){
        this->ui->action3->setEnabled(true);
    });
    QString ip = this->sts->getip();
    QString ipp = "服务器ip地址 : ";
    ipp += ip;
    QMessageBox::information(this,"创建服务器",ipp);
}


void MainWindow::on_action_2_triggered()
{
    bool bRet = false;
    QRegularExpression re(QRegularExpression::anchoredPattern(R"((?:(?:[01]?\d?\d|2[0-4]\d|25[0-5])\.){3}(?:[01]?\d?\d|2[0-4]\d|25[0-5]))"));
    while(true){
        QString enterip = QInputDialog::getText(this, tr("连接至服务器"), tr("请输入服务器端ip地址："), QLineEdit::Normal,0,&bRet);
        if(!bRet){
            break;
        }
        QRegularExpressionMatch im = re.match(enterip);
        if(im.hasMatch()){
            this->sts = new NetGame(false,enterip);
            connect(this->sts,&NetGame::StartSceneBack,this,[=](){
                this->sts->hide();
                this->show();
                this->sts->cdtimer->stop();
                this->sts->cdtimereset();
            });
            break;
        }
    }
    connect(this->sts,&NetGame::cancli,this,[=](){
        this->ui->action3->setEnabled(true);
    });
}


void MainWindow::on_action_3_triggered()
{
    this->sts1 = new Ingame();
    this->sts1->_nowgreen  = 1;
    connect(this->sts1,&Ingame::StartSceneBack,this,[=](){
        this->sts1->hide();
        this->show();
        this->sts1->cdtimer->stop();
        this->sts1->cdtimereset();
    });
    this->hide();
    this->sts1->initscene1();
    this->sts1->show();
    this->sts1->cdtimer->start();
}

