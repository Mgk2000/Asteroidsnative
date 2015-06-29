/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
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
#include <cstdio>
View::View() :
     bullets(0),asteroidAppearTime(0), nticks(0), period(12),
	 ship(0), gun(0), pause(false)
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
				if (!asteroid->isSplinter())
					createSplinters(asteroid);
				_scores+=asteroid->cost();
				delete asteroid;
				delete bullet;
				sound(2);
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
		if (!bullet->isMy() && (ship->isPointInside(&p) || ship->isPointInside(&p1)))
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
		if (patrol)
		{
			delete patrol;
			patrol = 0;
		}
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
            Asteroid* asteroid = new Asteroid (this, textures[0]);
            asteroid->init();
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
            breakShip();
            break;
        }
    }
}

int View::drawFrame(long long currTime)
{
    if (nticks ==0)
    {
        startTime = currTime;
        lastTime = currTime;
    }
    float delta = (float)(currTime - lastTime)/1000;
    if (delta>0.1) //big delay after breaking GL
    {
    	startTime = startTime+ currTime - lastTime;
    	lastTime=currTime;
    	return 0;
    }
    nticks = (currTime-startTime) / period;
    lastTime = currTime;
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
        checkAppearences();
        checkAsteroidBreaksShip();
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
        for (int i = 0; i< textures.size(); i++)
            textures[i]->initGL();
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
        for (int i = 0; i< textures.size(); i++)
            textures[i]->initGL();
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
	Bullet* bullet = new Bullet(this, x,y,angle);
	bullet->init();
	addBullet (bullet);
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
        Splinter* splinter = new Splinter(this, textures[0]);
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
   // background->draw();
	ship->draw();
	gun->draw();
	for (std::list<Bullet*> ::iterator bit = bullets.begin(); bit != bullets.end(); bit++)
		(*bit)->draw();
    for (std::list<Asteroid*> ::iterator ait = asteroids.begin(); ait != asteroids.end(); ait++)
        (*ait)->draw();
	if (patrol)
		patrol->draw();
	drawCurrentResult();
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

void View::addTexture(const char* filename)
{
    Texture* texture = new Texture (filename);
    textures.push_back(texture);
    background = new Background(this, textures[0]);
    background->init();
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
