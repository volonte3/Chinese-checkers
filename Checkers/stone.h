#ifndef STONE_H
#define STONE_H

#include <QPixmap>
#include <QRect>

class Stone
{
public:
    Stone(bool isblue,bool ifc = 0);
    void setPosition(double x,double y,int num);
    QPixmap _stone;
    QPixmap _stonesel;
    bool check;
    int _num;
    bool _blue;
    double _x;
    double _y;
    QRect _rect;
};

#endif // STONE_H
