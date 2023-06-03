#ifndef NETGAME_H
#define NETGAME_H

#include "ingame.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>
#include <QMessageBox>
#include <QtGlobal>
#include <QRandomGenerator>
#include <QLabel>
#include <QLCDNumber>
#include <QMenuBar>
#include <QTimer>

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

#endif // NETGAME_H
