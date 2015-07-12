#include "view.h"
#include "logmsg.h"
#include "bitmaptext.h"
static float barX = -0.5;
static float barY  = -0.9;
static float cellWidth = 0.3;
static float cellDx = 0.005;
#define MAX_BONUSES 5
#include <vector>


void View::setShipBonus(Bonus* bonus, int msec)
{
    if (bonus != 0)
    {
        switch(bonus->kind())
        {
        case Bonus::SUPER_GUN:
        case Bonus::DIAMOND:
           if (_shipBonus!= 0)
                delete _shipBonus;
            _shipBonus = bonus;
            _shipBonusExpiredTime =currTime() + msec*1000;
            _shipBonus->setRotateAngle(0.0);
            shipBonus()->setX(-0.1);
            shipBonus()->setY(0.85);
            shipBonus()->setScale(2.0);
            break;
        case Bonus::BOMB:
            if (_shipBonuses.size() < MAX_BONUSES)
            {
                bonus->setY(barY + cellDx);
                bonus->setRotateAngle(0.0);
                _shipBonuses.push_back(bonus);
                break;
            }
            else
                delete bonus;
        default:
            break;
        }
    }
    else
    {
        if (_shipBonus)
        {
            delete _shipBonus;
            _shipBonus = 0;
        }
    }
}

void View::checkBonusExpired()
{
    if (_shipBonus!= 0)
        if (currTime() > _shipBonusExpiredTime)
            setShipBonus(0,0);
}

void View::drawShipBonuses()
{
    if (_shipBonus != 0)
    {
        float y = 0.75;
        int sec = (int) (_shipBonusExpiredTime - _currTime) /1000;
        char buf[8];
        sprintf(buf, " %d", sec);
//        text->draw(0.10 , y, 0.025, Point4D(1.0, 0.0,0.8, 1.0), 2.0, buf );
        Color col;
        if (sec>5)
            col = COLOR_GREEN;
        else
            col = COLOR_RED;
        bitmapText->draw (0.05 , y, 0.05, col, buf );
        shipBonus()->setY(y);
        shipBonus()->setX(-0.05);
        shipBonus()->setScale(1.5);
        shipBonus()->draw();

    }
    drawBonusBar();
}
void View::drawBonusBar()
{
    int ns = _shipBonuses.size();
    if (ns > 0)
    {
        for (int i =0; i< ns; i++)
        {
            float x = barX + i * cellWidth + cellDx;
            float y = barY + cellDx;
            Bonus* bonus = shipBonuses().at(i);
            bonus->setX(x);
            bonus->draw();
        }
    }
}

bool View::shipUnbreakable() const
{
    return shipBonusKind() == Bonus::DIAMOND;
}

void View::clearShipBonuses()
{
    if (_shipBonus)
        delete _shipBonus;
    _shipBonus = 0;
    for (int i =0; i< _shipBonuses.size(); i++)
        delete _shipBonuses[i];
    _shipBonuses.clear();
}
Bonus::Kind View::shipBonusKind() const
{
    if (!_shipBonus)
        return Bonus::NONE;
    else
        return _shipBonus->kind();
}
void View::deleteShipBonus(Bonus* bonus)
{
    if (_shipBonus == bonus)
    {
        delete _shipBonus;
        _shipBonus = 0;
        return;
    }
    std::vector<Bonus*>::iterator bit = _shipBonuses.begin();
    for ( ; bit != _shipBonuses.end(); bit++)
        if (*bit == bonus)
        {
            bit = _shipBonuses.erase(bit);
            delete bonus;
            break;
        }
}
void View::catchBonus(Bonus* bonus)
{
    Bonus::Kind kind = bonus->kind();
    sound(BONUS);
    switch (kind)
    {
    case Bonus::BOMB :
        LOGD("BigBomb kind=%d catched=%d",(int)kind - 1, _levelBonuses[(int)kind - 1]->catched+1 )
    case Bonus::SHOOTER:
    case Bonus::DIAMOND:
    case Bonus::SUPER_GUN:
         setShipBonus(bonus, 30);
         break;
    case Bonus::LIVE:
        _lives++;
        delete bonus;
        break;
    default:
        break;
    }
    _levelBonuses[(int)kind - 1]->catched++;

}
Bonus* View::touchedShipBonus(float x, float y)
{
    for (int i = 0; i< _shipBonuses.size(); i++)
    {
        //LOGD ("x=%f y=%f", x,y);
        float bx = barX + i* cellWidth;
        if (x > bx && x< bx+ cellWidth && y > barY-cellWidth && y< barY + cellWidth)
            return _shipBonuses[i];
    }
    return 0;
}
