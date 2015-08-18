#include "view.h"
#include <math.h>
#include "ship.h"
#include "gun.h"
#include "bullet.h"
#include "asteroid.h"
#include "patrol.h"
#include "patrolbullet.h"
#include "mutex.h"
#include "logmsg.h"
#include "background.h"
#include "bonus.h"
#include <cstdio>
#include <time.h>
#include "math_helper.h"
#include "sand.h"
#include "explosion.h"
#include "target.h"
#include "rectangle.h"
#include "roundedrectangle.h"
#include "bitmaptext.h"
#include "texture.h"
#include "shooter.h"
#include "pausecontinue.h"
View::View() :
     bullets(0),asteroidAppearTime(0), nticks(0), period(12),
     ship(0), gun(0), _pause(false), _shipBonus(0), _smallExplosionRadius(0.5),
     _maxTargets(20), _gameRunning(false)
{
    initLevels();
}

View::~View()
{
	delete ship;
	delete gun;
	delete mutex;

}

void View::processTouchMove(int x, int y)
{
	float fx, fy;
	screenToView(x,y, &fx, &fy);
	if (ship->touched(fx, fy))
		ship->setX(fx);
}
long long lastExplosionTime = 0;
void View::processTouchPress(int x, int y)
{
    float fx,fy;
    screenToView(x,y, &fx, &fy);
    if (_showingDialog)
    {
        processDialogTouch(fx, fy);
        return;
    }
    if (_pause)
    {
        pauseContinue->processTouchPause(fx, fy);
        return;
    }
	float fi;
     Bonus * bonus = touchedShipBonus(fx, fy);
    if ( bonus )
    {
        if (bonus->kind() == Bonus::BOMB)
        {
        	long long tt = currTime();
        	if (abs(tt-lastExplosionTime) < 200)
        		return;
            deleteShipBonus(bonus);
            bigExplosion();
            lastExplosionTime = tt;
        }
    }
    else if (gun->touched(fx, fy, &fi))
    {
    	long long tt = currTime();
    	long delta = tt  - okPressTime;
        LOGD("First shoot delta=%d", delta);
        if (delta > 300)
        shoot (fi);
    }
    else
        pauseContinue->processTouchPause(fx, fy);

}
void View::destroyAsteroid(Asteroid* asteroid, bool total)
{
    if (asteroid->bonus())
        freeBonus(asteroid);
    else if (! total && !asteroid->isSplinter())
        createSplinters(asteroid);
    else
    {
        Sand* sand = new Sand(this, asteroid);
        sands.push_back(sand);
    }
    _scores+=asteroid->cost();
    delete asteroid;
    sound(BREAK);

}

void View::smallExplosion()
{
    std::list<Asteroid*>::iterator ait = asteroids.begin();
    float er2 = sqr(_smallExplosionRadius);
    float x = ship->X();
    float y = ship->top();
    for (; ait != asteroids.end(); )
    {
        Asteroid* asteroid = *ait;
        float ax = asteroid->X();
        float ay = asteroid->Y();
        float r2 = sqr(x-ax) + sqr(y-ay);
        if (r2 <= er2)
        {
            destroyAsteroid(asteroid, true);
            ait = asteroids.erase(ait);
        }
        else
            ait++;
    }
    Explosion * explosion = new Explosion(this, _smallExplosionRadius);
    explosions.push_back(explosion);
    sound(EXPLOSION);
}

void View::bigExplosion()
{
    Explosion * explosion = new Explosion(this, 2.0);
    explosions.push_back(explosion);
    sound(EXPLOSION);
    std::list<Asteroid*>::iterator ait = asteroids.begin();
    for (; ait != asteroids.end(); ait++)
    {
        Asteroid* asteroid = *ait;
        {
            destroyAsteroid(asteroid, true);
        }
    }
    asteroids.clear();
    std::list <Bullet*>::iterator bit = bullets.begin();
    for (; bit != bullets.end(); bit++)
        delete (*bit);
    bullets.clear();
    if (patrol)
    {
        _scores += patrol->cost();
        _patrolBreaks++;
        delete patrol;
        patrol = 0;
    }
}

void View::checkShoots()
{
	for (std::list<Bullet*> ::iterator bit = bullets.begin(); bit != bullets.end(); bit++)
	{
		Point p = (*bit)->top();
		Point p1 = (*bit)->bottom();
		Bullet* bullet = *bit;
        if (bullet->isMy())
        {
            for (int i=0; i< _levelTargets; i++)
            if (targets[i] && (targets[i]->isPointInside(&p) ||
                               targets[i]->isPointInside(&p1)))
            {
//                LOGD("patrol delete bullets=%d", bullets.size() );
                targets[i]->shooted();
                if (targets[i]->broken())
                {
                    freeBonus(targets[i]);
                    Sand* sand = new Sand(this, targets[i]);
                    sands.push_back(sand);

                    delete targets[i];
                    targets[i] = 0;
                }
                delete *bit;
                bit = bullets.erase(bit);
                sound(BREAK);
                goto nextbullet;

            }

        }
		for (std::list<Asteroid*> ::iterator ait = asteroids.begin(); ait != asteroids.end(); ait++)
		{
			if ((*ait)->isPointInside(&p ) || (*ait)->isPointInside(&p1))
			{
                Asteroid* asteroid = *ait;
                asteroid->shooted();
                if (asteroid->broken())
                {
                    destroyAsteroid(asteroid, false);
                    ait = asteroids.erase(ait);
                }
                bit = bullets.erase(bit);
                delete bullet;
                goto nextbullet;
            }
		}
		if (bullet->isMy())
		if (patrol && (patrol->isPointInside(&p) || patrol->isPointInside(&p1)))
		{
            //LOGD("patrol delete bullets=%d", bullets.size() );
			_scores+= patrol->cost();
            Sand* sand = new Sand(this, patrol, Color(0, 0.5, 1));
            sands.push_back(sand);
			delete patrol;
            _patrolBreaks++;
			patrol =0;
			delete *bit;
			bit = bullets.erase(bit);
            sound(BREAK);
			goto nextbullet;

		}
        if (!bullet->isMy() && !shipUnbreakable() &&
                (ship->isPointInside(&p) || ship->isPointInside(&p1)))
			breakShip();
		nextbullet: ;
	}
}

void View::patrolShoot(Patrol *_patrol)
{
	float angle = atan2(ship->X() - _patrol->X(), ship->Y() - _patrol->Y());
	PatrolBullet* bullet = new PatrolBullet(this, _patrol->X() , _patrol->Y() , angle);
	bullet->init();
	addBullet (bullet);
}

void View::breakShip()
{
	if (ship->dead())
		return;
	ship->die();
    _lives--;
    if (_lives)
        sound(SHIPBREAK);
    else sound(ENDGAME);
	dieticks = 100;
    nticks = 0;
}

void View::processTouches()
{
	mutex->lock();
    int nt = touches.size();
    for (std::list<TouchEvent>::const_iterator it2 = touches.begin();
         it2 != touches.end(); it2++)
        {
            const TouchEvent &te = *it2;
            switch(te.type)
            {
            case TouchPointPressed:
                processTouchPress(te.x, te.y);
                break;
            case TouchPointMoved:
                processTouchMove(te.x, te.y);
                break;
            default: break;
            }
        }
    touches.clear();
    mutex->unlock();
}
void View::newGame()
{
	LOGD("View::newGame() 1");
	if (ship)
	{
		LOGD("View::newGame() 2");
        clearGame();
		LOGD("View::newGame() 3");
		initLevels();
		startGame();
		LOGD("View::newGame() 4");
	}
	LOGD("View::newGame() 5");
}
void View::startGame()
{
    _level = 0;
    _lives = 3;
    nticks = 0;
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _showingDialog = true;
//    showLevelDialog();
    //startLevel(0);
}
void View::checkEndGame()
{
    if (ship->dead())
    {
        dieticks --;
        if (!dieticks)
        {
            if( !gameIsOver())
                ship->revive();
        }
    }

}

void View::moveObjects(float delta)
{
    for (int i=0; i< _levelTargets; i++)
        if (targets[i])
            targets[i]->moveStep(delta);
    for (std::list<Bullet*> ::iterator bit = bullets.begin(); bit != bullets.end(); bit++)
    {
        (*bit)->moveStep(delta);
        if ((*bit)->out() )
        {
            delete *bit;
            bit = bullets.erase(bit);
        }
    }
    for (std::list<Asteroid*> ::iterator ait = asteroids.begin(); ait != asteroids.end(); ait++)
    {
        (*ait)->moveStep(delta);
        if ((*ait)->out() )
        {
            delete *ait;
            ait = asteroids.erase(ait);
        }
    }
    for (std::list<Bonus*> ::iterator ait = bonuses.begin(); ait != bonuses.end(); ait++)
    {
        (*ait)->moveStep(delta);
        if ((*ait)->out() )
        {
            delete *ait;
            ait = bonuses.erase(ait);
        }
    }
    for (std::list<Sand*> ::iterator ait = sands.begin(); ait != sands.end(); ait++)
    {
        (*ait)->moveStep(delta);
        if ((*ait)->out() )
        {
            delete *ait;
            ait = sands.erase(ait);
        }
    }
    for (std::list<Explosion*> ::iterator ait = explosions.begin(); ait != explosions.end(); ait++)
    {
        (*ait)->moveStep(delta);
        if ((*ait)->out() )
        {
            delete *ait;
            ait = explosions.erase(ait);
        }
    }
    if (patrol)
    {
        patrol->moveStep(delta);
        if (patrol->out())
        {
            delete patrol;
            patrol = 0;
        }
    }
}

void View::checkAppearences()
{
    if (nticks >= asteroidAppearTime)
    {
        bool pat = false;
        if (!patrol)
        {
            if (_random1.frandom() <= 0.3)
            {
                patrol = new Patrol (this, _textures[(int)Bonus::PATROL]);
                patrol->init();
                sound(PATROL);
                pat = true;
            }
        }
        if (!pat)
        {
            Bonus* bonus = 0;
            Asteroid* asteroid = new Asteroid (this, _textures[0]);
            asteroid->init();
            float rr =_random1.frandom();
            if (rr < _levelBonusProbability)
//            if (rr < 0.5)
            {
                float fb = _random1.frandom();
                Bonus::Kind bk;
                if (fb<=0.4)
                    bk = Bonus::BOMB;
                else if (fb<=0.55)
                    bk = Bonus::SHOOTER;
                else if (fb<=0.7)
                    bk = Bonus::SUPER_GUN;
                else if (fb<=0.85)
                	bk = Bonus::DIAMOND;
                else
                	bk = Bonus::LIVE;
//                bk = Bonus::SHOOTER;
                if (bk != Bonus::SHOOTER)
                    bonus = new Bonus(this, bk, asteroid);
                else
                    bonus = new Shooter(this, asteroid);
                asteroid->setBonus(bonus);
                bonus->init();
            }
            addAsteroid(asteroid);
        }
        asteroidAppearTime = nticks + random1().irandom(300, 700) *
           _levelAppearencePeriod  / log10 (nticks+10.0) ;
    }
}

void View::checkAsteroidBreaksShip()
{
    for (std::list<Asteroid*> ::iterator ait = asteroids.begin(); ait != asteroids.end(); ait++)
    {
        if (ship->isIntersects(**ait))
        {
            if (!shipUnbreakable())
                breakShip();
            else
            {
                destroyAsteroid(*ait, false);
                ait = asteroids.erase(ait);
            }
            break;
        }
    }
}

void View::checkCatchBonus()
{
    for (std::list<Bonus*>::iterator bit = bonuses.begin();
         bit != bonuses.end(); bit++)
    {
        if (ship->isIntersects(**bit))
        {
            catchBonus(*bit);
            bit = bonuses.erase(bit);
        }
    }
}
void View::freeBonus(Asteroid* asteroid)
{
    if (asteroid->bonus())
    {
        bonuses.push_back(asteroid->bonus());
        asteroid->bonus()->free();
        asteroid->setBonus(0);
    }
}


int View::drawFrame()
{
	//LOGD("DrawFrame 1 pause=%d", _pause);
#ifdef WIN32
    _currTime = 1.0* clock() / CLOCKS_PER_SEC * 1000;
#else
    timeval tv;
    gettimeofday(&tv, 0);
    _currTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif

    if (nticks ==0)
    {
        _startTime = _currTime;
        _lastTime = _currTime;
    }
    float delta = (float)(_currTime - _lastTime)/1000;
    if (delta>0.1) //big delay after breaking GL
    {
        _startTime =_startTime+ _currTime - _lastTime;
        _lastTime=_currTime;
    	return 0;
    }
    nticks = (_currTime-_startTime) / period;
    _lastTime = _currTime;
    maxSound =NONE;
    checkEndGame();
    if (gameIsOver())
    {
        if (!dieticks)
            return -1;
    }
    else if (_gameRunning && !_showingDialog && !_pause)
    {
        processTouches();
        moveObjects(delta);
        checkShoots();
        checkAsteroidBreaksShip();
        checkBonusExpired();
        checkCatchBonus();
        checkAppearences();
        if (checkLevelDone())
        {
        }
    }
    else if (_showingDialog || _pause)
        processTouches();
    paintGL();
	nticks ++;
	//LOGD("DrawFrame Exit");
	return maxSound;
}


bool View::initializeGL(bool _russian)
{
    //LOGD("View::initializeGL() 1 level=%d levelDone=%d maxsound=%d ship=%d", _level, (int)_levelDone, (int) maxSound, (int) ship);
   russian = _russian;
	glClearColor(0.0,0., 0.1, 1);
   // LOGD("View::initializeGL() 2");
    if (!initShaders())
        return false;
    //LOGD("View::initializeGL() 3");

    if (!ship)
    {
      //  LOGD("View::initializeGL() 4");
        _textureProgram = new TextureProgram();
        for (int i = 0; i< _textures.size(); i++)
            _textures[i]->initGL(_textureProgram->program());
        //LOGD("View::initializeGL() 4.1  _textures.size()=%d" , _textures.size());
        background = new Background(this, _textures[7]);
        background->init();
        //LOGD("View::initializeGL() 4.2");

        ship = new Ship (this, _textures[(int) Bonus::SHIP]);
		gun = new Gun (this);
		patrol = 0;
		// Use QBasicTimer because its faster than QTimer
        //text = new Text(this);
        //LOGD("View::initializeGL() 4.3 Bonus::LETTERS=%d", (int)Bonus::LETTERS);
        bitmapText = new BitmapText (this, _textures[(int)Bonus::LETTERS]);
        bitmapText->init();
        //LOGD("View::initializeGL() 4.4");
        _rectangle = new ARectangle(this);
        _roundedRect = new RoundedRectangle(this, 1, 1.2);
        _wideRoundedRect = new RoundedRectangle(this, 0.4, 0.15);
        //LOGD("View::initializeGL() 4.5");
        pauseContinue = new PauseContinue(this, _textures[(int) Bonus::PAUSE], _textures[(int) Bonus::CONTINUE]);
        pauseContinue->init();
        mutex = new Mutex;
		startGame();
        //LOGD("View::initializeGL() 4.6");

    }
    else //after pause pressing Home button)
    {
        //LOGD("View::initializeGL() 5");
    	_textureProgram->initGL();
        for (unsigned int i = 0; i< _textures.size(); i++)
            _textures[i]->initGL(_textureProgram->program());
        //LOGD("View::initializeGL() 6");
        ship->initGL();
        //LOGD("View::initializeGL() 7");
    	gun->initGL();
        //LOGD("View::initializeGL() 8");
        //text->initGL();
        //LOGD("View::initializeGL() 8.1");
        delete bitmapText;
        //LOGD("View::initializeGL() 8.11");

        bitmapText = new BitmapText (this, _textures[(int)Bonus::LETTERS]);
        //LOGD("View::initializeGL() 8.12");
        bitmapText->init();
        //LOGD("View::initializeGL() 9");
        _rectangle->initGL();
        _roundedRect->initGL();
        _wideRoundedRect->initGL();
        pauseContinue->initGL();
        for (int i=0; i< targets.size(); i++)
        	if (targets[i])
        	targets[i]->initGL();
        //LOGD("View::initializeGL() 10");
        //background->initGL();
        if (!ship->dead())
    	{
			if (patrol)
				patrol->initGL();
          //  LOGD("View::initializeGL() 11");
			std::list<Bullet*>::iterator bit = bullets.begin();
			for(; bit!= bullets.end(); bit++)
				(*bit)->initGL();
            //LOGD("View::initializeGL() 12");
			std::list<Asteroid*>::iterator ait = asteroids.begin();
			for(; ait!= asteroids.end(); ait++)
				(*ait)->initGL();
            //LOGD("View::initializeGL() 13");
            for(std::list<Bonus*>::iterator ait = bonuses.begin();
                ait!= bonuses.end(); ait++)
                (*ait)->initGL();
        	//LOGD("View::initializeGL() 14");
            for(std::vector<Bonus*>::iterator ait = _shipBonuses.begin();
                ait!= _shipBonuses.end(); ait++)
                (*ait)->initGL();
            //LOGD("View::initializeGL() 15");
        }
    }
    return true;
}

//! [3]
bool View::initShaders()
{
	const char* vertexstr =
	"#ifdef GL_ES\n"
	"// Set default precision to medium\n"
	"precision mediump int;\n"
	"precision mediump float;\n"
	"#endif\n"
	"attribute vec3 aVertexPosition;\n"
	"uniform mat4 mvp_matrix;\n"
	"void main(void) {\n"
	"	gl_Position = mvp_matrix * vec4(aVertexPosition, 1.0);\n"
	"	}\n";

	const char* fragstr =
	"#ifdef GL_ES\n"
	"// Set default precision to medium\n"
	"precision mediump int;\n"
	"precision mediump float;\n"
	"#endif\n"
	"uniform vec4 color;\n"
	"void main(void) {\n"
	"	  gl_FragColor = color;\n"
	"	}\n";
	_program = createProgram(vertexstr, fragstr);
	if (!_program)
        return false;

	_colorlocation = glGetUniformLocation(_program, "color");
	_matrixlocation = glGetUniformLocation(_program, "mvp_matrix");
	_vertexlocation = glGetAttribLocation(_program, "aVertexPosition");
    return true;
}

void View::screenToView(int x, int y, float *fx, float *fy) const
{
    *fx = 2.0 * (x - width/2) / width * aspect;
    *fy = - 2.0 * (y - height/2) * 1.0 / height;
}

void View::shoot(float angle)
{
	float x = ship->X();
	float y = ship->top();
    if (shipBonusKind() == Bonus::SUPER_GUN)
    {
        float dfi = 0.3;
        int np = 5;
        for (int i =0; i< np; i++)
        {
            Bullet* bullet = new Bullet(this, x,y,angle - dfi/2.0 + dfi/np*i);
            bullet->init();
            addBullet (bullet);
        }
    }
    else
    {
        Bullet* bullet = new Bullet(this, x,y,angle);
        bullet->init();
        addBullet (bullet);
    }
    sound(SHOOT);
}

void View::addAsteroid(Asteroid *asteroid)
{
	asteroids.push_back(asteroid);
}

void View::deleteAsteroid(Asteroid *asteroid)
{
	for (std::list<Asteroid*> ::iterator bit = asteroids.begin(); bit != asteroids.end(); bit++)
	{
		if (*bit == asteroid)
		{
			delete asteroid;
			asteroids.erase(bit);
			break;
		}
	}
}

void View::addBullet(Bullet *bullet)
{
	bullets.push_back(bullet);
}

void View::deleteBullet(Bullet *bullet)
{
	for (std::list<Bullet*> ::iterator bit = bullets.begin(); bit != bullets.end(); bit++)
	{
		if (*bit == bullet)
		{
			delete bullet;
			bullets.erase(bit);
			break;
		}
	}
}

void View::createSplinters(Asteroid* asteroid)
{
	int nsp = _random2.irandom(3,5);
	for (int i =0; i< nsp; i++)
	{
        Splinter* splinter = new Splinter(this, _textures[0]);
		float fi = M_PI * i / nsp;
		splinter->init(*asteroid, fi);
		asteroids.push_front(splinter);
	}
}


void View::resizeGL(int w, int h)
{
    width = w;
    height = h;
	glViewport(0, 0, w, h);
	aspect = w * 1.0 / h;
	projection1.setToIdentity();
	//ortho
	projection1.m[0][0] = 1.0 / aspect;
	projection1.m[2][2] = -1.0;

}

void View::paintGL()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //background->draw();
    //glViewport(0, 0.0, 380,700);
    if (_showingDialog)
    {
        showLevelDialog();
        return ;
    }
	ship->draw();
	gun->draw();
    for (int i=0; i<_levelTargets; i++)
        if (targets[i])
            targets[i]->draw();
	for (std::list<Bullet*> ::iterator bit = bullets.begin(); bit != bullets.end(); bit++)
		(*bit)->draw();
    for (std::list<Asteroid*> ::iterator ait = asteroids.begin(); ait != asteroids.end(); ait++)
        (*ait)->draw();
    for (std::list<Bonus*> ::iterator ait = bonuses.begin(); ait != bonuses.end(); ait++)
        (*ait)->draw();
    for (std::list<Sand*> ::iterator ait = sands.begin(); ait != sands.end(); ait++)
        (*ait)->draw();
    for (std::list<Explosion*> ::iterator ait = explosions.begin(); ait != explosions.end(); ait++)
        (*ait)->draw();
    if (patrol)
		patrol->draw();
	drawCurrentResult();
    pauseContinue->draw();
    drawLevelTodo();

    drawShipBonuses();
//    background->draw();
    if (gameIsOver())
    {
        _gameRunning = false;
        drawEndGame();
    }
    if (_levelDone)
    {
        levelDoneTicks --;
        if (levelDoneTicks >=0)
            drawLevelDone();
        else
        {
            clearGame();
            showLevelDialog();
        }
    }
}
void View::setPPause(bool p)
{
	 _pause = p;
//	 LOGD("View::setPause %d", (int) _pause);
}
void View::drawCurrentResult() const
{
	char buf[32];
	if (!russian)
		std::sprintf(buf, "Level:%d Scores:%d Lives:%d", _level+1, _scores, _lives);
	else
		std::sprintf(buf, "Óðîâåíü:%d Î÷êè:%d Æèçíè:%d", _level+1, _scores, _lives);

    _rectangle->draw(-0.6,  0.9, 0.6, 1.0, Point4D(0.2, 0.0, 0.5));
    float scale = 0.035;
    bitmapText->draw(-0.57, 0.94, scale, Point4D(1.0,1.0,0.0),  buf);
    //bitmapText->draw(-0.57, 0.84, scale, Point4D(1.0,1.0,0.0), "Å¨ÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏ");
    //bitmapText->draw(-0.57, 0.74, scale, Point4D(1.0,1.0,0.0), "ÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßß");
   // bitmapText->draw(-0.57, 0.64, scale, Point4D(1.0,1.0,0.0), "àáâãäå¸æçèéêëìíîï");
    //bitmapText->draw(-0.57, 0.54, scale, Point4D(1.0,1.0,0.0), "ïðñòóôõö÷øùúûúýþÿÿ");
    //bitmapText->draw(-0.57, 0.44, scale, Point4D(1.0,1.0,0.0), "ABCDEFGHIJKLMNOPP");
    //bitmapText->draw(-0.57, 0.34, scale, Point4D(1.0,1.0,0.0), "QRSTUVWXYZ)!@#$^&*()");
    //bitmapText->draw(-0.57, 0.24, scale, Point4D(1.0,1.0,0.0), "abcdefghijklmnopp");
   // bitmapText->draw(-0.57, 0.14, scale, Point4D(1.0,1.0,0.0), "qrstuvwxyz01234567890");

}
void View::drawEndGame() const
{
	if (!russian)
		bitmapText->drawCenter(0, 0.0, 0.05, COLOR_RED, "END");
	else
		bitmapText->drawCenter(0, 0.0, 0.05, COLOR_RED, "ÊÎÍÅÖ ÈÃÐÛ");
}

void View::drawLevelDone() const
{
    char buf[64];
    if (!russian)
    	sprintf(buf, "Level %d done!", _level);
    else
    	sprintf(buf, "Óðîâåíü %d ïðîéäåí!", _level);
    bitmapText ->drawCenter(0, 0, 0.05, COLOR_GREEN, buf);
}

void View::addTexture(int w, int h, const char* data, int kind, bool transparentWhite)
{
    if ((int) kind ==0)
    {
    	if (_textures.size() != 0)
    		_textures.clear();
    }
	Texture* texture = new Texture (w,h, data, kind, transparentWhite);
    _textures.push_back(texture);
   // LOGD("View::addTexture textures.size =%d", _textures.size());
}
void View::onTouchEvent(int what, int x, int y)
{
	mutex->lock();
    TouchEvent te (what, x, y);
    touches.push_back(te);
    mutex->unlock();
}

GLuint View::createShader(GLenum shaderType, const char *src)
{
	GLuint shader = glCreateShader(shaderType);
	if (!shader) {
		return 0;
	}
	glShaderSource(shader, 1, &src, NULL);

	GLint compiled = GL_FALSE;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}
GLuint View::createProgram(const char* vtxSrc, const char* fragSrc) {
	GLuint vtxShader = 0;
	GLuint fragShader = 0;
	GLuint program = 0;
	GLint linked = GL_FALSE;

	vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
	if (!vtxShader)
		goto exit;

	fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
	if (!fragShader)
		goto exit;

	program = glCreateProgram();
	if (!program) {
		goto exit;
	}
	glAttachShader(program, vtxShader);
	glAttachShader(program, fragShader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{

		glDeleteProgram(program);
		program = 0;
	}

exit:
	glDeleteShader(vtxShader);
	glDeleteShader(fragShader);
	return program;
}
