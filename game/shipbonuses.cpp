#include "view.h"

void View::freeBonus(Asteroid* asteroid)
{
    bonuses.push_back(asteroid->bonus());
    asteroid->bonus()->free();
}

void View::setShipBonus(Bonus* bonus, int msec)
{
    _shipBonus = bonus;
    if (Bonus != 0)
        _shipBonusExpiredTime =currTime() + msec*1000;
}

void View::checkBonusExpired()
{
    if (_shipBonus!= Bonus::NONE)
        if (currTime() > _shipBonusExpiredTime)
        {
            _delete _shipBonus;
            setShipBonus(0,0);
        }
}

void View::drawShipBonuses()
{
    if (_shipBonus != Bonus::NONE)
    {

    }
}
