#include "netgame.h"
#include "config.h"
#include "qlabel.h"
#include <QDebug>

#define cout qDebug()

NetGame::NetGame(bool iserver,QString ip)
{
    ishost = iserver;
    ready = 0;
    hready = 0;
    _server = NULL;
    _socket = NULL;

    if(iserver){
        this->ishost = 1;
        _server = new QTcpServer(this);
        _server->listen(QHostAddress::Any, 3031);
        connect(_server,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
    }
    else{
        this->ishost = 0;
        _socket = new QTcpSocket(this);
        _socket->connectToHost(QHostAddress(ip), 3031);

        connect(_socket, SIGNAL(readyRead()),this,SLOT(slotRecv()));
    }
}

QString NetGame::getip()
{
    QString ip;
    QString hostname = QHostInfo::localHostName();
    QHostInfo hostinfo = QHostInfo::fromName(hostname);
    QList<QHostAddress> iplist = hostinfo.addresses();
    if(!iplist.empty()){
        for(int i = 0; i < iplist.length(); i++){
            QHostAddress ahost = iplist.at(i);
            if(QAbstractSocket::IPv4Protocol == ahost.protocol()){
                ip = ahost.toString();
                break;
            }
        }
    }
    return ip;
}

void NetGame::getready()
{
    this->ready = 1;
    char buf[1];
    buf[0] = 3;
    _socket->write(buf, 1);
}

void NetGame::test()
{
    if(ishost){
        cout << "host";
    }
    else{
        cout << "guest";
    }
}

void NetGame::initpst()
{
    pst[0] = new PPoint(INI_X,GREEN_INI_Y,0);
    pst[1] = new PPoint(INI_X-20,GREEN_INI_Y-34.375,0);
    pst[2] = new PPoint(INI_X+20,GREEN_INI_Y-34.375,0);
    pst[3] = new PPoint(INI_X-40,GREEN_INI_Y-68.75,0);
    pst[4] = new PPoint(INI_X,GREEN_INI_Y-68.75,0);
    pst[5] = new PPoint(INI_X+40,GREEN_INI_Y-68.75,0);
    pst[6] = new PPoint(INI_X-60,GREEN_INI_Y-103.125,0);
    pst[7] = new PPoint(INI_X-20,GREEN_INI_Y-103.125,0);
    pst[8] = new PPoint(INI_X+20,GREEN_INI_Y-103.125,0);
    pst[9] = new PPoint(INI_X+60,GREEN_INI_Y-103.125,0);

    pst[120] = new PPoint(INI_X,BLUE_INI_Y,0);
    pst[118] = new PPoint(INI_X-20,BLUE_INI_Y+34.375,0);
    pst[119] = new PPoint(INI_X+20,BLUE_INI_Y+34.375,0);
    pst[115] = new PPoint(INI_X-40,BLUE_INI_Y+68.75,0);
    pst[116] = new PPoint(INI_X,BLUE_INI_Y+68.75,0);
    pst[117] = new PPoint(INI_X+40,BLUE_INI_Y+68.75,0);
    pst[111] = new PPoint(INI_X-60,BLUE_INI_Y+103.125,0);
    pst[112] = new PPoint(INI_X-20,BLUE_INI_Y+103.125,0);
    pst[113] = new PPoint(INI_X+20,BLUE_INI_Y+103.125,0);
    pst[114] = new PPoint(INI_X+60,BLUE_INI_Y+103.125,0);

    for (int i = 0; i < 13 ;i ++){
        pst[i+10] = new PPoint(INI_X-(6-i)*40,GREEN_INI_Y-137.5,0);
    }
    for (int i = 0; i < 13 ;i ++){
        pst[110-i] = new PPoint(INI_X+(6-i)*40,BLUE_INI_Y+137.5,0);
    }
    for (int i = 0; i < 12 ;i ++){
        pst[i+23] = new PPoint(INI_X-20-(5-i)*40,GREEN_INI_Y-171.875,0);
    }
    for (int i = 0; i < 12 ;i ++){
        pst[97-i] = new PPoint(INI_X-20+(6-i)*40,BLUE_INI_Y+171.875,0);
    }
    for (int i = 0; i < 11 ;i ++){
        pst[i+35] = new PPoint(INI_X-(5-i)*40,GREEN_INI_Y-206.25,0);
    }
    for (int i = 0; i < 11 ;i ++){
        pst[85-i] = new PPoint(INI_X+(5-i)*40,BLUE_INI_Y+206.25,0);
    }
    for (int i = 0; i < 10 ;i ++){
        pst[i+46] = new PPoint(INI_X-20-(4-i)*40,GREEN_INI_Y-240.625,0);
    }
    for (int i = 0; i < 10 ;i ++){
        pst[74-i] = new PPoint(INI_X-20+(5-i)*40,BLUE_INI_Y+240.625,0);
    }
    for (int i = 0; i < 9 ;i ++){
        pst[i+56] = new PPoint(INI_X-(4-i)*40,GREEN_INI_Y-275,0);
    }
    if(ishost){
        for(int i = 0; i < 10 ; i ++){
            stone[i]->_x = pst[i]->x;
            stone[i]->_y = pst[i]->y;
            stone[i]->_num = i;
            pst[i]->haves = 1;
        }
        for(int i = 10; i < 20 ; i ++){
            stone[i]->_x = pst[i+101]->x;
            stone[i]->_y = pst[i+101]->y;
            stone[i]->_num = i + 101;
            pst[i+101]->haves = 1;
        }
    }
    else{
        for(int i = 0; i < 10 ; i ++){
            stone[i]->_x = pst[120-i]->x;
            stone[i]->_y = pst[120-i]->y;
            stone[i]->_num = 120 - i;
            pst[120 - i]->haves = 1;
        }
        for(int i = 10; i < 20 ; i ++){
            stone[i]->_x = pst[19-i]->x;
            stone[i]->_y = pst[19-i]->y;
            stone[i]->_num = 19 - i;
            pst[19-i]->haves = 1;
        }
    }
}

void NetGame::checkwin()
{
    int greenct = 0;
    int bluect = 0;
    if(ishost){
        for(int i = 0; i < 10; i++){
            if(stone[i]->_y < 139){
                greenct++;
            }
        }
        for(int i = 10; i < 20; i++){
            if(stone[i]->_y > 481){
                bluect++;
            }
        }
        if(greenct == 10)
            gwin();
        if(bluect == 10)
            bwin();
    }
    else{
        for(int i = 10; i < 20; i++){
            if(stone[i]->_y < 139){
                bluect++;
            }
        }
        for(int i = 0; i < 10; i++){
            if(stone[i]->_y > 481){
                greenct++;
            }
        }
        if(greenct == 10)
            gwin();
        if(bluect == 10)
            bwin();
    }
}

void NetGame::checkleave()
{
    int bb = 0;
    int gg = 0;
    if(ishost){
        for(int i = 0; i < 10; i++){
            if(stone[i]->_y < 481.875){
                gg++;
            }
        }
        for(int i = 10; i < 20; i++){
            if(stone[i]->_y > 138.125){
                bb++;
            }
        }
        this->_greenleave = gg;
        this->_blueleave = bb;
    }
    else{
        for(int i = 10; i < 20; i++){
            if(stone[i]->_y < 481.875){
                bb++;
            }
        }
        for(int i = 0; i < 10; i++){
            if(stone[i]->_y > 138.125){
                gg++;
            }
        }
        this->_greenleave = gg;
        this->_blueleave = bb;
    }
}

void NetGame::click(int clickedid, int _id)
{
    if(_selid == -1 && clickedid != -1){
        if(ishost && clickedid >= 10){
            return;
        }
        if(!ishost && clickedid < 10){
            return;
        }
    }


    char buf[3];
    buf[0] = 1;
    buf[1] = clickedid;
    buf[2] = 120 - _id;
    _socket->write(buf, 3);

    Ingame::click(clickedid, _id);


}

void NetGame::cancelclick()
{
    Ingame::cancelclick();

    char buf[1];
    buf[0] = 2;
    _socket->write(buf, 1);
}

void NetGame::initscene1()
{
    setWindowTitle(GAME_TITLE);
    setWindowIcon(QIcon(":/res/game.ico"));
    setFixedSize(GAME_WIDTH,GAME_HEIGHT);


    for(int i = 0; i < 10 ; i ++){
        this->stone[i] = new Stone(false);
    }
    for(int i = 10; i < 20 ; i ++){
        this->stone[i] = new Stone(true);
    }
    initpst();


    QLCDNumber * lcdcd = new QLCDNumber;
    lcdcd->setParent(this);
    lcdcd->setGeometry(QRect(670,100,120,40));
    lcdcd->setDigitCount(2);
    lcdcd->display(20);
    connect(this,&Ingame::CDTimeChange,this,[=](int nowtime){
       lcdcd->display(nowtime);
    });


    QLabel * roundlb = new QLabel;
    QLabel * roundtx = new QLabel;
    QLabel * lefttm = new QLabel;
    QLabel * hostcolor = new QLabel;
    QLabel * clientcolor = new QLabel;
    QFont font1("华文新魏",30);
    QFont font2("华文新魏",31);
    QFont font3("华文隶书",15);

    if(ishost){
        hostcolor->setParent(this);
        hostcolor->setGeometry(QRect(670,210,150,40));
        hostcolor->setText("我方棋子 : 绿色");
        hostcolor->setFont(font3);
        clientcolor->setParent(this);
        clientcolor->setGeometry(QRect(670,270,150,40));
        clientcolor->setText("对方棋子 : 蓝色");
        clientcolor->setFont(font3);
    }
    else{
        hostcolor->setParent(this);
        hostcolor->setGeometry(QRect(670,270,150,40));
        hostcolor->setText("对方棋子 : 绿色");
        hostcolor->setFont(font3);
        clientcolor->setParent(this);
        clientcolor->setGeometry(QRect(670,210,150,40));
        clientcolor->setText("我方棋子 : 蓝色");
        clientcolor->setFont(font3);
    }

    roundtx->setParent(this);
    roundtx->setGeometry(QRect(670,370,120,40));
    roundtx->setText("回合数");
    roundtx->setFont(font2);

    roundlb->setFont(font1);
    roundlb->setParent(this);
    roundlb->setGeometry(QRect(715,415,50,35));
    roundlb->setText(QString::number(_nowround));
//    cout << _nowround;
    connect(this,&Ingame::RoundValueChange,this,[=](int newRound){
        roundlb->setText(QString::number(newRound));
    });

    lefttm->setParent(this);
    lefttm->setGeometry(QRect(650,50,160,40));
    lefttm->setText("剩余时间");
    lefttm->setFont(font2);


    QMenuBar* bar = new QMenuBar(this);
    QMenu * startmenu = bar->addMenu("游戏");
    loseAction = startmenu->addAction("认输  (第20回合后生效)");
    loseAction->setEnabled(false);
    connect(loseAction,&QAction::triggered,this,[=](){
        char buf[1];
        buf[0] = 5;
        _socket->write(buf, 1);
        if(ishost)
            bwin();
        else
            gwin();
    });
}

void NetGame::slotNewConnection()
{
    if(_socket)
        return;
    _socket = _server->nextPendingConnection();
    connect(_socket, SIGNAL(readyRead()),this,SLOT(slotRecv()));
    emit this->cancli();
    QMessageBox::information(this,"连接至服务器","连接服务器成功");
    this->_nowgreen = QRandomGenerator::global()->bounded(2);
    char buf[2];
    buf[0] = 4;
    buf[1] = this->_nowgreen;
    _socket->write(buf, 2);
}

void NetGame::slotRecv()
{
    QByteArray ba = _socket->readAll();
    if(ba[0] == 1){
        int clkid = ba[1];
        int psid = ba[2];
        Ingame::click(clkid,psid);
    }
    else if(ba[0] == 2){
        Ingame::cancelclick();
    }
    else if(ba[0] == 3){
        this->hready = 1;
        emit this->okready();
    }
    else if(ba[0] == 4){
        this->_nowgreen = ba[1];
        emit this->cancli();
    }
    else if(ba[0] == 5){
        if(ishost)
            gwin();
        else
            bwin();
    }
}
