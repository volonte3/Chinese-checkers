#include "stone.h"
#include "config.h"
Stone::Stone(bool isblue,bool ifc)
{
    check = ifc;
    if(isblue){
        _stone.load(ST2_PATH);
        _stonesel.load(ST2SEL_PATH);
        this->_blue = true;
    }
    else{
        _stone.load(ST1_PATH);
        _stonesel.load(ST1SEL_PATH);
        this->_blue = false;
    }
    _stone = _stone.scaled(_stone.height()*0.22,_stone.width()*0.22);
    _stonesel = _stonesel.scaled(_stonesel.height()*0.22,_stonesel.width()*0.22);
    _x = 310;
    _y = 585;
    _rect.setHeight(this->_stone.height());
    _rect.setWidth(this->_stone.width());
    _rect.moveTo(this->_x,this->_y);
}

void Stone::setPosition(double x, double y, int num)
{
    this->_x = x;
    this->_y = y;
    this->_rect.moveTo(x,y);
    this->_num = num;
}
