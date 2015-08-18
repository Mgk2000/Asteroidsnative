#include "view.h"
#include "target.h"
#include "flyingobject.h"
#include <vector>
#include <list>
#include <math.h>
#include "ship.h"
#include "bullet.h"
#include "patrol.h"
#include "math_helper.h"
#include "roundedrectangle.h"
#include "bitmaptext.h"
#include "logmsg.h"
void View::initLevels()
{
    levelRandom = new Random();
    int nb = (int) Bonus::MAX_BONUS;
    for (int i = 0; i< nb; i++)
    {
        LevelBonus* lb = new LevelBonus;
        _levelBonuses.push_back(lb);
    }
    strcpy(_levelBonuses[(int) Bonus::BOMB -1]->name, "BB");
    strcpy(_levelBonuses[(int) Bonus::SHOOTER -1]->name, "sh");
    strcpy(_levelBonuses[(int) Bonus::SUPER_GUN -1]->name, "sg");
    strcpy(_levelBonuses[(int) Bonus::DIAMOND -1]->name, "d");
    strcpy(_levelBonuses[(int) Bonus::LIVE -1]->name, "live");
    _maxCountedLevel = 4;
    for (int i = 0; i< _maxTargets; i++)
    {
        targets.push_back(0);
    }
}
void View::createTargets()
{
    for (int i =0; i< _levelTargets; i++)
    {
        float x = levelRandom->frandom(left()+ 0.05, this->right() -0.05);
        float y = levelRandom->frandom(0.4, 0.80);
        float r = levelRandom->frandom(0.03, 0.05);
        float r2 = sqr(0.05);
        if (i > 0)
        {
            for (;;)
            {
                bool found = false;
                for (int j=0; j<i ; j++)
                    if (sqr(x - targets[j]->X()) + sqr(y - targets[j]->Y()) < r2)
                    {
                        found = true;
                        break;
                    }
                if (found)
                {
                    x = levelRandom->frandom(left()+ 0.05, this->right() -0.05);
                    y = levelRandom->frandom(0.4, 0.80);
                }
                else
                    break;
            }
        }
        Texture* t = textures().at(0);
        targets[i] = new Target(this, t, x, y, r);
        //Bonus* tbonus = new Bonus(this, Bonus::TARGET, target);
        //targets[i]->setBonus(tbonus);
    }
    for (int i= _levelTargets; i< _maxTargets; i++)
    {
        if (targets[i])
        {
            delete targets[i];
            targets[i] = 0;
        }
    }
}

void View::calcLevel(int __level)
{
    int clevel = __level % _maxCountedLevel;
    //_levelAppearenceFrequency = calcAppearenceFrequency(__level);
    _levelBonusProbability = 0.1;
    _levelAppearencePeriod = 1.5 / log( (__level+1)*2.718281828);
    for (int i =0 ; i < _levelBonuses.size(); i++)
    {
        _levelBonuses[i]->num = 0;
        _levelBonuses[i]->catched = 0;
    }
    _levelStartScores = _scores;
    /*
    int _level;
    int _maxCountedLevel;
    float _appearenceFrequency;
    int _levelTargets;
    int _levelPatrolBreaks, _patrolBreaks;
    std::vector<int> _levelBonuses;
    int _levelScores;
    int _levelTargetBreaks;
    bool _levelDone;
    std::vector<int> _catchedBonuses;
    Random * levelRandom;
    */
    //from 0 - when draw, +1
    switch (clevel)
    {
    case 0:
        _levelTargets = 1;
        _levelPatrolBreaks =0;
        _levelTargetBreaks = 10;
        _levelScores = 200;
        _levelStartScores = 0;
        _levelPatrolBreaks =1;
        break;
    case 1:
        _levelTargets = 2;
        _levelPatrolBreaks =0;
        _levelTargetBreaks = 20;
        _levelBonuses[1]->num = 1;
        _levelScores = 1000;
        break;
    case 2:
        _levelTargets = 1;
        _levelPatrolBreaks = 0;
        _levelTargetBreaks = 20;
        _levelBonuses[0]->num = 2;
        _levelBonuses[1]->num = 2;
        _levelBonuses[2]->num = 2;
        _levelBonuses[3]->num = 2;
        _levelBonuses[4]->num = 2;
        _levelScores = 3000;
        //_levelBonusProbability = 0.2;
        break;
    case 3:
        _levelTargets = 0;
        _levelPatrolBreaks = 30;
        _levelTargetBreaks = 20;
        _levelScores = 0;
        break;
    case 4:
        _levelTargets = 6;
        _levelPatrolBreaks =0;
        _levelTargetBreaks = 10;
        _levelScores = 500;
        for (int i =0 ; i < _levelBonuses.size(); i++)
            _levelBonuses[i]->num = 2;
        _levelPatrolBreaks =12;
        break;
    }
    createTargets();
}
float View::calcAppearenceFrequency(int __level)
{
    return 1.0;
}
void View::startLevel(int l)
{
    _level = l;
    _random1.reset();
    _random2.reset();
//    touches.clear();
    ship->revive();
    nticks = 0;
    ship->setX(0.0f);
    asteroidAppearTime = nticks+1;
    _lives = 3;
    if (_level ==0)
        _scores = 0;
    _patrolBreaks = 0;
    _random1.reset();
    for (int i=0; i< _levelTargets; i++)
    {
        if (targets[i] ==0)
            createTargets();
        targets[i]->setBreakCount(_levelTargetBreaks);
        targets[i]->setShootCount(0);
        targets[i]->init();
    }
    for (int i=0; i< _levelBonuses.size(); i++)
        _levelBonuses[i]->catched=0;
    _gameRunning = true;
//    _showingDialog = true;
}

void View::clearGame()
{
	//LOGD("ClearGame 1");
    if (patrol)
    {
    	//LOGD("ClearGame 2");
        delete patrol;
        patrol = 0;
    	//LOGD("ClearGame 3");

    }
    for (int i=0; i< targets.size(); i++)
    {
    	//LOGD("ClearGame 4");
        if(targets[i])
        {
            delete targets[i];
            targets[i] = 0;
        }
    }
	//LOGD("ClearGame 5");
    for (std::list<Bullet*>::iterator bit = bullets.begin();
         bit!= bullets.end(); bit++)
        delete *bit;
    bullets.clear();
	//LOGD("ClearGame 6");
	for (std::list<Asteroid*>::iterator bit = asteroids.begin();
         bit!= asteroids.end(); bit++)
        delete *bit;
    asteroids.clear();
	//LOGD("ClearGame 7");
    for (std::list<Bonus*>::iterator bit = bonuses.begin();
         bit!= bonuses.end(); bit++)
        delete *bit;
    bonuses.clear();
	//LOGD("ClearGame 8");

    for (std::vector<Bonus*>::iterator bit = _shipBonuses.begin();
         bit!= _shipBonuses.end(); bit++)
        delete *bit;
    _shipBonuses.clear();
	//LOGD("ClearGame 9");
    if (_shipBonus)
    {
        delete _shipBonus;
        _shipBonus = 0;
    }
    //_scores = 0;
	//LOGD("ClearGame 10");

}

bool View::checkLevelDone()
{
    //bool done = false;
    for (int i =0; i< _levelTargets; i++)
        if (targets[i] && targets[i]->breakCount()>0)
            return false;
    if (_patrolBreaks < _levelPatrolBreaks)
        return false;
    for (int i =0; i< _levelBonuses.size(); i++)
        if ( _levelBonuses[i]->catched < _levelBonuses[i]->num)
            return false;
    if (_scores - _levelStartScores <_levelScores)
        return false;
    _lives=1;
    sound(LEVELDONE);
    _levelDone = true;
    _gameRunning = false;
    levelDoneTicks = 100;
    _level++;
    return true;
}

void View::drawLevelTodo()
{
    float left = -0.55;
    float top = -0.3;
    float tleft = left + 0.02;
    float ttop = top;
    float dh = -0.08;
    float scale = 0.03;
    if (russian)
    	bitmapText->draw(tleft, ttop,scale,Color(1,1,0),"НАДО:");
    else
    	bitmapText->draw(tleft, ttop,scale,Color(1,1,0),"TODO:");
    ttop = ttop + dh;
    char buf[128];
    if (_levelScores && _levelScores - (_scores-_levelStartScores) >0)
    {
    	if (!russian)
    		sprintf(buf, "Scores:%d", _levelScores - (_scores-_levelStartScores));
    	else
    		sprintf(buf, "Очки:%d", _levelScores - (_scores-_levelStartScores));
        bitmapText->draw(tleft,ttop,scale,Color(1,1,0), buf);
        ttop = ttop + dh;
    }
    if (_levelTargets)
    {
        int nt =0;
        for (int i=0; i<_levelTargets; i++)
            if (targets[i])
                nt ++;
        if (nt)
        {
            sprintf (buf,"%%%d%%  %d", (int) Bonus::TARGET, nt);
            bitmapText->draw(tleft,ttop,scale,Color(1,1,0), buf);
            ttop = ttop + dh;
        }
    }
    if (_levelPatrolBreaks && _levelPatrolBreaks - _patrolBreaks >0)
    {
        sprintf(buf, "%%%d%%  %d", (int) Bonus::PATROL, _levelPatrolBreaks - _patrolBreaks);
        bitmapText->draw(tleft,ttop,scale,Color(1,1,0), buf);
        ttop = ttop + dh;
    }
    for (int i =0; i< _levelBonuses.size(); i++)
    {
        LevelBonus* lb = _levelBonuses[i];
        if (lb->num && lb->catched < lb->num)
        {
            sprintf(buf, "%%%d%%  %d",  i + 1,
                    lb->num - lb->catched);
            bitmapText->draw(tleft,ttop,scale,Color(1,1,0), buf);
            ttop = ttop + dh;
        }
    }
}
void View::showLevelDialog()
{
   _lives = 1999;
    //startLevel(2);
    calcLevel(_level);
    _showingDialog =  true;
    float top = 0.8;
    float left = -0.4;
    float bottom = -0.8;
    float dh = -0.15;
    int nrows = 0;
    if (_levelDone)
        nrows++;
//    nrows++; //level X
    nrows++; //your task;
    if (_levelScores)
        nrows++;
    if (_levelTargets)
            nrows++;
    if (_levelPatrolBreaks)
        nrows++;
    bool levelBonusesFlag = false;
    for (int i =0; i<_levelBonuses.size(); i++)
        if (_levelBonuses[i]->num)
        {
            nrows++;
            levelBonusesFlag = true;
            break;
        }
    float y = top - 0.2;
    char buf[128];
    float scale = 0.04;
    _roundedRect->drawFramed(-0.5f,  bottom, 0.5f, top, 3.0f,
                             Point4D(0.3, 0.2, 0.0), Point4D(1.0, 1.0, 0.0));
    if (_level)
    {
        sprintf(buf, "Level %d done!", _level);
        bitmapText->draw(-0.2, y,scale, Color(1,1,1),buf);
    }
    y = y + dh;
    if (!russian)
    	sprintf(buf, "Your task for level %d:", _level+1);
    else
    	sprintf(buf, "Задание на уровень %d:", _level+1);
    bitmapText->draw(left, y,scale, Color(1,1,1), buf);
    y = y + dh;
    if (_levelScores)
    {
    	if (!russian)
    		sprintf (buf, "Scores : %d", _levelScores);
    	else
    		sprintf (buf, "Очки : %d", _levelScores);
        bitmapText->draw(left, y,scale, Color(1,1,1),buf);
        y = y + dh;
    }
    if (_levelPatrolBreaks)
    {
        sprintf(buf, "%%%d%%     %d",(int) Bonus::PATROL, _levelPatrolBreaks);
        bitmapText->draw(left, y,scale, Color(1,1,1),buf);
        y = y + dh;
    }
    buf[0] = 0;
    char buf1[16];
    if (_levelTargets)
    {
    	if (!russian)
    		sprintf (buf, "%%%d%%    %d  Shoots : %d",
                 (int) Bonus::TARGET, _levelTargets, _levelTargetBreaks);
    	else
    		sprintf (buf, "%%%d%%    %d  Попадания : %d",
                 (int) Bonus::TARGET, _levelTargets, _levelTargetBreaks);

        bitmapText->draw(left, y,scale, Color(1,1,1),buf);
        y = y + dh;
    }
    buf[0] = 0;
    if (levelBonusesFlag)
    {
        for (int i =0; i< _levelBonuses.size(); i++)
        {
            if (_levelBonuses[i]->num)
            {
                sprintf(buf1, "%%%d%%  %d ", i+1, _levelBonuses[i]->num);
                strcat(buf, buf1);
            }
        }
        bitmapText->draw(left, y,scale, Color(1,1,1),buf);
        y = y + dh;
    }
    _okButtonLeft = -0.2;
    _okButtonRight = -_okButtonLeft;
    _okButtonBottom = bottom + 0.1;
    _okButtonTop = _okButtonBottom + 0.1;
    float _okLeft = -0.02;
    float _okTop = _okButtonBottom + 0.035;

    _wideRoundedRect->drawFramed(_okButtonLeft,  _okButtonBottom, _okButtonRight, _okButtonTop,
         1.0, Point4D(0.0, 0.0, 0.0), Point4D(1.0, 1.0, 0.0));
    bitmapText->draw(_okLeft, _okTop , 0.06, Point4D(1,1,0), "OK");
}

void View::processDialogTouch(float fx, float fy)
{
    if (fx >= _okButtonLeft-0.1  && fx <= _okButtonRight+0.1  &&
        fy >= _okButtonBottom-0.1  && fy <=_okButtonTop+0.1
            )
    {
    	okPressTime = currTime();
    	LOGD("Ok pre4ssed fx=%f fy=%f left=%f right=%f bot=%f top=%f", fx, fy, _okButtonLeft, _okButtonRight,
    			_okButtonBottom, _okButtonTop);
    	_showingDialog = false;
        _levelDone = false;
         startLevel(_level);
    }
}
