#include "ingame.h"
#include "ui_ingame.h"
#include "config.h"
#include <QPainter>
#include <QPixmap>
#include <QMenuBar>
#include <QMouseEvent>
#include <QSoundEffect>
#include <QDebug>
#include <QLabel>
#include <QLCDNumber>
#include <QTimer>
#include <QMessageBox>
#include <QAbstractButton>
#include <cmath>
#define cout qDebug()

Ingame::Ingame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ingame)
{
    ui->setupUi(this);

    _selid = -1;
    _oripos = -1;
    _nowround = 1;
    _count = 0;
    _bluetime = 0;
    _greentime = 0;
    _blueleave = 0;
    _greenleave = 0;
    _nowsecond = 20;

    this->windlg = new WinDialog(this);
    this->losedlg = new LoseDialog(this);
    connect(this->windlg,&WinDialog::winback,this,[=](){
        restart();
        emit this->StartSceneBack();
    });
    connect(this->losedlg,&LoseDialog::loseback,this,[=](){
       restart();
       emit this->StartSceneBack();
    });


    this->cdtimer = new QTimer(this);
    cdtimer->setInterval(1000);
    connect(cdtimer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
    eff1 = new QSoundEffect();
    eff1->setSource(QUrl::fromLocalFile(SEL_SOUND));
    eff1->setLoopCount(1);
    eff1->setVolume(0.25f);
    eff2 = new QSoundEffect();
    eff2->setSource(QUrl::fromLocalFile(SET_SOUND));
    eff2->setLoopCount(1);
    eff2->setVolume(0.25f);
    eff3 = new QSoundEffect();
    eff3->setSource(QUrl::fromLocalFile(FSEL_SOUND));
    eff3->setLoopCount(1);
    eff3->setVolume(3.0);
    still = new QSoundEffect();
    still->setSource(QUrl::fromLocalFile(STILL_SOUND));
    still->setLoopCount(1);
    still->setVolume(0.25f);
    warning = new QSoundEffect();
    warning->setSource(QUrl::fromLocalFile(WARNING_SOUND));
    warning->setLoopCount(1);
    warning->setVolume(0.25f);
}

Ingame::~Ingame()
{
    delete ui;
}

void Ingame::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix1;
    pix1.load(":/res/map.png");
    painter.drawPixmap(0,0,pix1);
    for(int i = 0; i < 20; i++){
        drawStone(painter,i);
    }
    drawPos(painter);
    QPixmap pixgreen,pixblue,pixpy;
    pixgreen.load(GREEN_SIDE);
    pixblue.load(BLUE_SIDE);
    pixpy.load(NOW_PLAYER);
    pixpy = pixpy.scaled(pixpy.width() * 0.28,pixpy.height() * 0.32);
    pixgreen = pixgreen.scaled(pixgreen.width() * 0.4,pixgreen.height() * 0.4);
    pixblue = pixblue.scaled(pixblue.width() * 0.4,pixblue.height() * 0.4);
    painter.drawPixmap(650,470,pixpy);
    if(_nowgreen){
        painter.drawPixmap(635,500,pixgreen);
    }
    else{
        painter.drawPixmap(635,500,pixblue);
    }
}

void Ingame::drawPos(QPainter &painter){
    QPixmap pix2;
    pix2.load(OKSET_PATH);
    pix2 = pix2.scaled(pix2.width() * 0.5,pix2.height() * 0.5);
    for(int i = 0; i < 121; i++){
        if(pst[i]->okset){
            painter.drawPixmap(pst[i]->x-5,pst[i]->y-8,pix2);
        }
    }
}

void Ingame::drawStone(QPainter &painter, int _id)
{
    if(_id == _selid){
        painter.drawPixmap(stone[_id]->_x,stone[_id]->_y,stone[_id]->_stonesel);
    }
    else{
        painter.drawPixmap(stone[_id]->_x,stone[_id]->_y,stone[_id]->_stone);
    }
}

bool Ingame::getid(QPoint ppt, int& id)
{
       double _r = this->stone[0]->_rect.width() * 0.5;
       for(id = 0 ; id < 121 ; id++){
            double dx = ppt.x() - this->pst[id]->x;
            double dy = ppt.y() - this->pst[id]->y;
            double dist = dx * dx + dy * dy;
            if(dist <= _r * _r){
                return true;
            }
       }
       return false;
}

bool Ingame::cansel(int ori, int des)
{
    if(pst[ori]->next(*pst[des])){
        return true;
    }
    else{
        return false;
    }
}

int Ingame::line(int ori, int des)
{
    double cx = ( pst[ori]->x + pst[des]->x ) * 0.5;
    double cy = ( pst[ori]->y + pst[des]->y ) * 0.5;
    for(int i = 0; i < 20; i++){
        if(cansel(ori,stone[i]->_num) && cx == pst[stone[i]->_num]->x && cy == pst[stone[i]->_num]->y){
            return i;
        }
    }
    return -1;
}

void Ingame::bfs(int ori)
{
    for(int i = 0; i < 20; i++ ){
        if(!stone[i]->check && pst[ori]->next(*pst[stone[i]->_num])){
            stone[i]->check = 1;
            int orii = getline(stone[i]->_num,ori);
            if(!pst[orii]->haves){
                pst[orii]->okset = 1;
                bfs(orii);
            }
        }
    }
}

int Ingame::getline(int center, int ori)
{
    double fx = pst[center]->x * 2 - pst[ori]->x;
    double fy = pst[center]->y * 2 - pst[ori]->y;
    for(int i = 0 ; i < 121 ; i++){
        if(pst[i]->x == fx && pst[i]->y == fy){
            return i;
        }
    }
    return -1;
}

void Ingame::checkleave()
{
    int bb = 0;
    int gg = 0;
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

void Ingame::test()
{
    cout << "normal";
}

void Ingame::onTimeOut()
{
    if(_nowsecond > 0){
        this->_nowsecond --;
        emit CDTimeChange(_nowsecond);
    }
    else{
        cdtimer->stop();
        if(_nowgreen){
            _greentime++;
            if(_greentime == 3){
                bwin();
            }
            else{
                QMessageBox * msg = new QMessageBox(this);
                msg->setText(QString("绿色方已经超时 %1 次，由蓝色方执棋").arg(_greentime));
                msg->setToolTip("超时警告");
                QTimer::singleShot(2500,msg,SLOT(accept()));
                msg->show();
                msg->button(QMessageBox::Ok)->hide();
                _selid = -1;
                _oripos = -1;
                _nowgreen = 1 - _nowgreen;
                cdtimereset();
                cdtimer->start();
                _count ++;
                if(_count % 2 == 0){
                    _nowround ++;
                    emit RoundValueChange(_nowround);
                }
                update();
                initpos();
            }
        }
        else{
            _bluetime++;
            if(_bluetime == 3){
                gwin();
            }
            else{
                QMessageBox * msg = new QMessageBox(this);
                msg->setText(QString("蓝色方已经超时 %1 次，由绿色方执棋").arg(_bluetime));
                msg->setToolTip("超时警告");
                QTimer::singleShot(2500,msg,SLOT(accept()));
                msg->show();
                msg->button(QMessageBox::Ok)->hide();
                _selid = -1;
                _oripos = -1;
                _nowgreen = 1 - _nowgreen;
                cdtimereset();
                cdtimer->start();
                _count ++;
                if(_count % 2 == 0){
                    _nowround ++;
                    emit RoundValueChange(_nowround);
                }
                update();
                initpos();
            }
        }
    }
}

void Ingame::showpos()
{
    int maxx = fmax(0,_oripos - 30);
    int minn = fmin(120,_oripos + 30);
    for(int i = maxx; i <= minn; i++){
        if(!pst[i]->haves && cansel(_oripos,i)){
            pst[i]->okset = 1;
        }
    }
    bfs(_oripos);
    update();
}

void Ingame::cdtimereset()
{
    this->cdtimer->stop();
    _nowsecond = 21;
}

void Ingame::tryselect(int clickedid, int _id)
{
    if(clickedid != -1){
        if(_nowgreen){
            if(clickedid < 10){
                this->_selid = clickedid;
                this->_oripos = _id;
                eff3->play();
                eff3->setVolume(3.0);
            }
        }
        else{
            if(clickedid >= 10){
                this->_selid = clickedid;
                this->_oripos = _id;
                eff3->play();
                eff3->setVolume(3.0);
            }
        }
    }
    update();
    pst[_oripos]->okset = 1;
    showpos();
//            cout << "1";
}

void Ingame::trymove(int clickedid, int _id)
{
    if(clickedid != -1){
        warning->play();
        return;
    }
    else{
        if(pst[_id]->okset){
            pst[stone[_selid]->_num]->haves = 0;
            stone[_selid]->setPosition(pst[_id]->x,pst[_id]->y,_id);
            pst[_id]->haves = 1;
            update();
            eff1->play();
//                        cout << "2";
        }
        else{
            warning->play();
            return;
        }
    }
}

void Ingame::tryset()
{
    checkleave();
    _selid = -1;
    eff2->play();
    _oripos = -1;
//                cout << "3";
    _nowgreen = 1 - _nowgreen;
    cdtimereset();
    cdtimer->start();
    _count ++;
    if(_count % 2 == 0){
        _nowround ++;
        emit RoundValueChange(_nowround);
    }
    if(_nowround >= 20){
        this->loseAction->setText("认输");
        this->loseAction->setEnabled(true);
    }
    if(_nowround == 20 && _greenleave < 5){
        bwin();
        return;
    }
    if(_nowround == 20 && _blueleave < 5){
        gwin();
        return;
    }
    if(_nowround == 25 && _greenleave < 8){
        bwin();
        return;
    }
    if(_nowround == 25 && _blueleave < 8){
        gwin();
        return;
    }
    if(_nowround == 30 && _greenleave < 10){
        bwin();
        return;
    }
    if(_nowround == 30 && _blueleave < 10){
        gwin();
        return;
    }
    update();
    initpos();
    checkwin();
}

void Ingame::checkwin()
{
    int greenct = 0;
    int bluect = 0;
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

void Ingame::click(int clickedid, int _id)
{
    if(this->_selid == -1){
        tryselect(clickedid, _id);
    }
    else{
        if(clickedid == _selid){
            tryset();
        }
        else{
            trymove(clickedid, _id);
        }
    }
}

void Ingame::cancelclick()
{
    pst[stone[_selid]->_num]->haves = 0;
    stone[_selid]->setPosition(pst[_oripos]->x,pst[_oripos]->y,_oripos);
    pst[_oripos]->haves = 1;
    update();
    _oripos = -1;
    _selid = -1;
    still->play();
    initpos();
//        cout << "still";
}

void Ingame::gwin()
{
    this->cdtimer->stop();
    this->windlg->show();
}

void Ingame::bwin()
{
    this->cdtimer->stop();
    this->losedlg->show();
}

void Ingame::initpos(){
    for(int i = 0; i < 121; i++){
        pst[i]->okset = 0;
    }
    for(int i = 0; i < 20; i++){
        stone[i]->check = 0;
    }
    update();
}

void Ingame::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton){
        QPoint pt = ev->pos();
        pt.setX(pt.x() - 20);
        pt.setY(pt.y() - 20);
        int _id;
        bool bRet = getid(pt,_id);
        if(!bRet){
            return;
        }
        int k = 0;
        int clickedid = -1;
        for(; k < 20; ++k){
            double pdx = stone[k]->_x;
            double pdy = stone[k]->_y;
            double yzx = pst[_id]->x;
            double yzy = pst[_id]->y;
            if(pdx == yzx && pdy == yzy){
                break;
            }
        }
        if(k < 20){
            clickedid = k;
        }
        click(clickedid, _id);
    }
    if(ev->button() == Qt::RightButton && _oripos != -1){
        cancelclick();
    }
}

void Ingame::restart()
{
    _oripos = -1;
    _selid = -1;
    _nowgreen = 1;
    _nowround = 1;
    _count = 0;
    _bluetime = 0;
    _greentime = 0;
    _blueleave = 0;
    _greenleave = 0;
    this->loseAction->setText("认输  (第20回合后生效)");
    this->loseAction->setEnabled(false);
    cdtimereset();
    initscene1();
    emit RoundValueChange(this->_nowround);
    update();
}

void Ingame::initscene1()
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
    QFont font1("华文新魏",30);
    QFont font2("华文新魏",31);

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
    QAction * backAction = startmenu->addAction("返回主页面");
    QAction * restartAction = startmenu->addAction("重新开始");
    loseAction = startmenu->addAction("认输  (第20回合后生效)");
    loseAction->setEnabled(false);
    connect(backAction,&QAction::triggered,this,[=](){
        restart();
        emit this->StartSceneBack();
    });
    connect(restartAction,&QAction::triggered,this,[=](){
        restart();
        this->cdtimer->start();
    });
    connect(loseAction,&QAction::triggered,this,[=](){
        if(_nowgreen)
            bwin();
        else
            gwin();
    });
}

void Ingame::initpst()
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
