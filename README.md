# 网络对战跳棋小游戏——设计文档



## 1.界面简介及游戏流程



游戏共有三个窗口：主窗口、游戏窗口、胜负弹窗。分别继承自`QMainWindow`类、`QWigdet`类、`QDialog`类。

主窗口及"游戏选择"菜单栏：

<img src="C:\Users\hangyu\AppData\Roaming\Typora\typora-user-images\image-20220823154905163.png" alt="image-20220823154905163" style="zoom:60%;" />

“游戏选择”中共有三个选项，可以选择作为服务器端创建联机，也可以作为客户端加入联机，还可以选择本地单机游戏。

点击“创建服务器连接”后，弹出本机ip地址信息窗口，等待客户端加入。

<img src="C:\Users\hangyu\AppData\Roaming\Typora\typora-user-images\image-20220823155502436.png" alt="image-20220823155502436" style="zoom:60%;" />

另一程序点击“连接至服务器”后，弹出输入ip地址的窗口，正确输入ip地址后即可建立连接；若输入非法ip地址，将会再次弹出该窗口要求用户重新输入。

<img src="C:\Users\hangyu\AppData\Roaming\Typora\typora-user-images\image-20220823155930262.png" alt="image-20220823155930262" style="zoom:60%;" />

当服务器端和客户端都进行游戏选择后，“开始游戏”菜单栏中的“准备游戏”会变为可选择状态，两者都点击该按钮后，游戏正式开始，进入游戏窗口，如下：

<img src="C:\Users\hangyu\AppData\Roaming\Typora\typora-user-images\image-20220824094919764.png" alt="image-20220824094919764" style="zoom:60%;" />

游戏窗口显示了当前执棋者，执棋者落子的剩余时间，双方的棋子颜色和当前回合数。“游戏”菜单栏中有“认输”的选项，在第20回合后可以选择认输。

下棋时，鼠标左键点击想要移动的棋子，会自动显示当前棋子可以跳到的所有位置，鼠标点击某一可用位置可将该棋子移到这个位置，再次鼠标左键点击可以落子。在选中及移动棋子时，可以点击鼠标右键取消选子或移动，重新选择要移动的棋子。

<img src="C:\Users\hangyu\AppData\Roaming\Typora\typora-user-images\image-20220824095559250.png" alt="image-20220824095559250" style="zoom:60%;" />

<img src="C:\Users\hangyu\AppData\Roaming\Typora\typora-user-images\image-20220824095748438.png" alt="image-20220824095748438" style="zoom:60%;" />

<img src="C:\Users\hangyu\AppData\Roaming\Typora\typora-user-images\image-20220824095639623.png" alt="image-20220824095639623" style="zoom:60%;" />

当执棋者20秒内没有完成落子动作时，会弹出警告窗口，并记录一次违规，违规三次时，自动判负。

<img src="C:\Users\hangyu\AppData\Roaming\Typora\typora-user-images\image-20220824095952821.png" alt="image-20220824095952821" style="zoom:70%;" />

当某一方胜利，或某一方认输，或三次时间违规，或指定回合时离开营地棋子过少时，均会触发胜负结算，弹出胜负窗口，告知玩家有息结果：

<img src="C:\Users\hangyu\AppData\Roaming\Typora\typora-user-images\image-20220824100306629.png" alt="image-20220824100306629" style="zoom:60%;" />

## 2.代码结构介绍

<img src="C:\Users\hangyu\AppData\Roaming\Typora\typora-user-images\image-20220824100609492.png" alt="image-20220824100609492" style="zoom:60%;" />

整个项目代码结构如图，除主函数所在的`main.cpp`外，`mainwindow.cpp/.h`设计主窗口，`ingame.cpp/.h`设计单机游戏窗口，`netgame.cpp/.h`设计网络游戏窗口，`windialog.cpp/.h`和`losedialog.cpp/.h`设计胜负窗口，`stone.h`设计棋子，`config.h`记录游戏参数，`res.qrc`保存游戏图像音频资源。

## 3.ui界面设计

ui界面设计主要通过纯代码的方式，用于初始化界面的函数`initscene1()`如下：

```cpp
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
```

显示的内容主要通过`QPixmap`和`QLabel`设置，倒计时显示采用了`QLCDNumber`，菜单栏使用了`QMenuBar`和`QMenu`。

## 4.服务器端与客户端工作流程

网络游戏类`NetGame`的组成如下：

```cpp
class NetGame : public Ingame
{
    Q_OBJECT
public:
    NetGame(bool iserver, QString ip = 0);

    QString getip();
    void getready();

    virtual void test();
    virtual void initpst();
    virtual void checkwin();
    virtual void checkleave();
    virtual void click(int clickedid, int _id);
    virtual void cancelclick();
    virtual void initscene1();

    QTcpServer* _server;
    QTcpSocket* _socket;
    bool ishost;
    bool ready;
    bool hready;
    int randf;

signals:
    void okready();
    void cancli();

public slots:
    void slotNewConnection();
    void slotRecv();

};
```

其继承自单机游戏类`Ingame`，通过重写基类虚函数的方式实现网络类的特定功能，并构造了`_socket`和`_server`两个变量用于实现网络通信。

网络游戏类`NetGame`的构造函数为：

```cpp
NetGame::NetGame(bool iserver,QString ip)
{
    ishost = iserver;
    ready = 0;
    hready = 0;
    _server = NULL;
    _socket = NULL;

    if(iserver){
        _server = new QTcpServer(this);
        _server->listen(QHostAddress::Any, 3031);
        connect(_server,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
    }
    else{
        _socket = new QTcpSocket(this);
        _socket->connectToHost(QHostAddress(ip), 3031);

        connect(_socket, SIGNAL(readyRead()),this,SLOT(slotRecv()));
    }
}
```

通过传入的参数`iserver`判断构造出的对象为服务器端还是客户端，若为服务器端，则创建`QTcpServer`指针`_server`，并监听所有ip地址，设置端口3031，并创建有客户端连接时的信号槽。若为客户端，则创建`QTcpSocket`指针`_socket`，并将其连接至输入的ip地址，创建收发网络传输信息的信号槽。

槽函数`slotNewConnection()`如下：

```c++
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
```

判断`_socket`是否为空防止多人连接，若为空则创建`_socket`与客户端相连，创建收发网络传输信息的信号槽，并发送先手方的信息。

服务器端和客户端之间传输字符数组，收取信息的槽函数`slotRecv()`如下：

```cpp
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
```

读取传输的信息后，通过字符数组的首位判断指令类型，1为鼠标点击事件（棋子的选取、移动、放置信息），2为取消鼠标点击（取消妻子选取或移动），3为准备游戏，4为先手方信息，5为胜负判定。若传输信息的字符数组长度大于1，则首位后的字符包含着该指令需要的参数信息。

## 5.信号与槽机制

下面就整个项目中除上述网络通信介绍过的之外使用到的信号与槽机制进行分析：



```cpp
connect(this->sts,&NetGame::okready,this,[=](){
        this->hide();
        this->sts->initscene1();
        this->sts->setGeometry(this->geometry());
        this->sts->show();
        this->sts->cdtimer->start();
    });
```

当对方准备按钮按下时，触发本方`okready`信号，进行隐藏主窗口、初始化游戏界面、打开游戏窗口、开始计时的操作



```cpp
connect(this->sts,&NetGame::cancli,this,[=](){
        this->ui->action3->setEnabled(true);
    });
```

成功建立连接后，触发`cancli`信号，将菜单中的“准备游戏”按钮设为可点击状态。



```cpp
connect(this->sts1,&Ingame::StartSceneBack,this,[=](){
        this->sts1->hide();
        this->show();
        this->sts1->cdtimer->stop();
        this->sts1->cdtimereset();
    });
```

单机游戏中，点击游戏窗口菜单栏中的“返回主菜单”按钮，触发`StartSceneBack`信号，隐藏游戏窗口，展示主窗口，重置游戏。



```cpp
connect(cdtimer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
```

当计时器触发`timeout`信号时，执行`onTimeOut()`槽函数，使当前时间减少一秒，并进行`emit CDTimeChange(_nowsecond)`操作。

```cpp
connect(this,&Ingame::CDTimeChange,this,[=](int nowtime){
       lcdcd->display(nowtime);
    });
```

时间改变后，LCD显示屏展示新的时间。