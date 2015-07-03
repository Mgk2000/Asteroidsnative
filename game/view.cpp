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
View::View() :
     bullets(0),asteroidAppearTime(0), nticks(0), period(12),
     ship(0), gun(0), pause(false), _shipBonus(0), _smallExplosionRadius(1.2)
{
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

void View::processTouchPress(int x, int y)
{
    float fx,fy;
    screenToView(x,y, &fx, &fy);
	float fi;
	if (gun->touched(fx, fy, &fi))
		shoot (fi);
    else
    {
        Bonus * bonus = touchedShipBonus(fx, fy);
        if (! bonus )
            return;
        if (bonus->kind() == Bonus::LITTLE_BOMB)
        {
            deleteShipBonus(bonus);
            smallExplosion();
        }
        else if (bonus->kind() == Bonus::BIG_BOMB)
        {
            deleteShipBonus(bonus);
            bigExplosion();
        }
    }

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
    sound(2);

}

void View::smallExplosion()
{
    std::list<Asteroid*>::iterator ait = asteroids.begin();
    float x = ship->X();
    float y = ship->Y();
    for (; ait != asteroids.end(); ait++)
    {
        Asteroid* asteroid = *ait;
        float ax = asteroid->X();
        float ay = asteroid->Y();
        float r2 = sqr(x-ax) + sqr(y-ay);
        if (r2 <= _smallExplosionRadius)
        {
            destroyAsteroid(asteroid, true);
            ait = asteroids.erase(ait);
            sound(2);
        }
    }
}

void View::bigExplosion()
{
    std::list<Asteroid*>::iterator ait = asteroids.begin();
    for (; ait != asteroids.end(); ait++)
    {
        Asteroid* asteroid = *ait;
        {
            destroyAsteroid(asteroid, true);
            sound(2);
        }
    }
    asteroids.clear();
    if (patrol)
    {
        _scores += patrol->cost();
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
		for (std::list<Asteroid*> ::iterator ait = asteroids.begin(); ait != asteroids.end(); ait++)
		{
			if ((*ait)->isPointInside(&p ) || (*ait)->isPointInside(&p1 ))
			{
                Asteroid* asteroid = *ait;
                ait = asteroids.erase(ait);
                bit = bullets.erase(bit);
                destroyAsteroid(asteroid, false);
                delete bullet;
                goto nextbullet;
			}
		}
		if (bullet->isMy())
		if (patrol && (patrol->isPointInside(&p) || patrol->isPointInside(&p1)))
		{
			LOGD("patrol delete bullets=%d", bullets.size() );
			_scores+= patrol->cost();
			delete patrol;
			patrol =0;
			delete *bit;
			bit = bullets.erase(bit);
			sound(2);
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
    	sound(4);
    else sound(5);
	dieticks = 100;
    nticks = 0;
}

void View::processTouches()
{
	mutex->lock();
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
	if (ship)
	{
		for (std::list<Asteroid*> ::iterator ait = asteroids.begin(); ait != asteroids.end(); ait++)
			delete *ait;
		asteroids.clear();
		for (std::list<Bullet*> ::iterator bit = bullets.begin(); bit != bullets.end(); bit++)
			delete *bit;
		bullets.clear();
        for (std::list<Sand*> ::iterator sit = sands.begin(); sit != sands.end(); sit++)
            delete *sit;
        sands.clear();
        if (patrol)
		{
			delete patrol;
			patrol = 0;
		}
        clearShipBonuses();
		startGame();
	}
}
void View::startGame()
{
	_random1.reset();
	_random2.reset();
    touches.clear();
	ship->revive();
    nticks = 0;
    ship->setX(0.0f);
	asteroidAppearTime = nticks+1;
	_lives = 3;
	_scores = 0;

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
                patrol = new Patrol (this);
                patrol->init();
                sound(3);
                pat = true;
            }
        }
        if (!pat)
        {
            Bonus* bonus = 0;
            Asteroid* asteroid = new Asteroid (this, _textures[0]);
            asteroid->init();
            if (_random1.frandom() < 0.5)
            {
                int n = _random1.irandom((int) Bonus::MAX_KIND) +1;
                //LOGD("kind=%d", n);
                bonus = new Bonus(this, (Bonus::Kind) n, asteroid);
                asteroid->setBonus(bonus);
                bonus->init();
            }
            addAsteroid(asteroid);
        }
        asteroidAppearTime = nticks + random1().irandom(300, 1000) / log10 (nticks+10.0);
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
    bonuses.push_back(asteroid->bonus());
    asteroid->bonus()->free();
}


int View::drawFrame()
{
    _currTime = 1.0* clock() / CLOCKS_PER_SEC * 1000;
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
    maxSound =0;
    checkEndGame();
    if (gameIsOver())
    {
        if (!dieticks)
            return -1;
    }
    else
    {
        processTouches();
        moveObjects(delta);
        checkShoots();
        checkAsteroidBreaksShip();
        checkBonusExpired();
        checkCatchBonus();
        checkAppearences();
    }
    paintGL();
	nticks ++;
	return maxSound;
}


bool View::initializeGL()
{
	glClearColor(0.0,0., 0.1, 1);
    if (!initShaders())
        return false;

    if (!ship)
    {
        for (int i = 0; i< _textures.size(); i++)
            _textures[i]->initGL();
        background = new Background(this, _textures[0]);
        background->init();

        ship = new Ship (this);
		gun = new Gun (this);
		patrol = 0;
		// Use QBasicTimer because its faster than QTimer
		text = new Text(this);
		mutex = new Mutex;
		startGame();
    }
    else //after pause 9pressing Home button)
    {
        for (int i = 0; i< _textures.size(); i++)
            _textures[i]->initGL();
        ship->initGL();
    	gun->initGL();
		text->initGL();
    	if (!ship->dead())
    	{
			if (patrol)
				patrol->initGL();
			std::list<Bullet*>::iterator bit = bullets.begin();
			for(; bit!= bullets.end(); bit++)
				(*bit)->initGL();
			std::list<Asteroid*>::iterator ait = asteroids.begin();
			for(; ait!= asteroids.end(); ait++)
				(*ait)->initGL();
            for(std::list<Bonus*>::iterator ait = bonuses.begin();
                ait!= bonuses.end(); ait++)
                (*ait)->initGL();
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
    sound(1);
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

	ship->draw();
	gun->draw();
	for (std::list<Bullet*> ::iterator bit = bullets.begin(); bit != bullets.end(); bit++)
		(*bit)->draw();
    for (std::list<Asteroid*> ::iterator ait = asteroids.begin(); ait != asteroids.end(); ait++)
        (*ait)->draw();
    for (std::list<Bonus*> ::iterator ait = bonuses.begin(); ait != bonuses.end(); ait++)
        (*ait)->draw();
    for (std::list<Sand*> ::iterator ait = sands.begin(); ait != sands.end(); ait++)
        (*ait)->draw();
    if (patrol)
		patrol->draw();
	drawCurrentResult();
    drawShipBonuses();
//    background->draw();
    if (gameIsOver())
        drawEndGame();
}
void View::drawCurrentResult() const
{
	char buf[32];
	std::sprintf(buf, "S:%d", _scores);
    text->draw(-0.4, 0.95, 0.025, Point4D(1.0,1.0,0.0),
               2.0, buf);
	std::sprintf(buf, "L:%d", _lives);
    text->draw(0.3, 0.95, 0.025, Point4D(0.0,1.0,0.0), 2.0, buf);

}
void View::drawEndGame() const
{
    text->draw(-0.07, 0.0, 0.05, Point4D(1.0,0.0,0.0),5.0, "END");
}

void View::addTexture(const char* data, int kind)
{
    Texture* texture = new Texture (data, kind);
    _textures.push_back(texture);
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
