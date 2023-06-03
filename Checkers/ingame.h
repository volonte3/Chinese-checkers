#ifndef INGAME_H
#define INGAME_H

#include <QWidget>
#include "stone.h"
#include <QMouseEvent>
#include <QSoundEffect>
#include <cmath>
#include "windialog.h"
#include "losedialog.h"

namespace Ui {
class Ingame;
}

class PPoint{
public:
    bool okset;
    bool haves;
    double x;
    double y;
    PPoint(double _x, double _y, bool okk, bool have = 0){
        x = _x;
        y = _y;
        okset = okk;
        haves = have;
    }
    bool next(PPoint other){
        double dx = this->x - other.x;
        double dy = this->y - other.y;
        if(dx * dx + dy * dy <= 1600){
            return true;
        }
        else
            return false;
    }
};

class Ingame : public QWidget
{
    Q_OBJECT

public:
    explicit Ingame(QWidget *parent = nullptr);
    ~Ingame();
    void paintEvent(QPaintEvent *);
    void drawStone(QPainter& painter, int _id);
    void drawPos(QPainter& painter);
    void restart();
    void initpos();

    void showpos();
    void cdtimereset();
    void mouseReleaseEvent(QMouseEvent *ev);
    void tryselect(int clickedid, int _id);
    void trymove(int clickedid, int _id);
    void tryset();
    void gwin();
    void bwin();
    bool getid(QPoint ppt, int& id);
    bool cansel(int ori, int des);
    int line(int ori, int des);
    void bfs(int ori);
    int getline(int center, int ori);

    virtual void checkleave();
    virtual void test();
    virtual void initpst();
    virtual void checkwin();
    virtual void click(int clickedid, int _id);
    virtual void cancelclick();
    virtual void initscene1();

    int _selid;
    int _oripos;
    bool _nowgreen;
    int _nowround;
    int _count;
    int _nowsecond;
    int _bluetime;
    int _greentime;
    int _greenleave;
    int _blueleave;
    Stone *stone[20];
    PPoint *pst[121];
    WinDialog *windlg;
    LoseDialog *losedlg;
    QTimer *cdtimer;
    QAction *loseAction;
    QSoundEffect *eff1;
    QSoundEffect *eff2;
    QSoundEffect *eff3;
    QSoundEffect *still;
    QSoundEffect *warning;

signals:
    void StartSceneBack();
    void RoundValueChange(int);
    void CDTimeChange(int);

public slots:
    void onTimeOut();

private:
    Ui::Ingame *ui;
};

#endif // INGAME_H
