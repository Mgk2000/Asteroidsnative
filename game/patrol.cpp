#include "patrol.h"
#include "view.h"
#include <math.h>
#include "logmsg.h"

Patrol::Patrol(View* _view, Texture* texture) : FlyingObject (_view, 2, texture),
    leftShoot(false), rightShoot(false), _firstDraw(true)
{
}

Patrol::~Patrol()
{

}
#define NP 24
void Patrol::init()
{
	bool left  = random1().frandom() <=0.5;
	y= 0.8;
	x = left ? view->left()-0.1 : view->right() + 0.1;
    vx = left ? 0.3 : -0.3;
	vy =0;
	_r = 0.05;
	width = _r * 1.3;
/*	nvertices = NP+2;
	vertices = new Point[NP+2];
	for (int i =0; i< NP; i++)
	{
		double fi = M_PI * i * 2 / (NP-1);
		vertices [i] = Point3D (_r * cos(fi) , _r * sin(fi) * 0.6, 0);
	}
	_color = Point4D (0.0, 0.7, 1.0, 1.0);
	width = _r * 1.3;
	vertices[NP]= Point3D (-width, 0.0, 0.0);
    vertices[NP+1]= Point3D (width, 0.0, 0.0);*/
    nvertices = 4;
    vertices = new Point[4];
    vertices[0] = Point(0,0.7 * _r);
    vertices[1] = Point(-1  * _r,0.0);
    vertices[2] = Point(0,-0.7 * _r);
    vertices[3] = Point(1 * _r,0);

	initGL();

}
void Patrol::initGL()
{
/*	glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
	glBufferData(GL_ARRAY_BUFFER, NP * sizeof(Point3D), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(Point3D), &vertices[NP], GL_STATIC_DRAW);
    */
    float _width = width;
    Point4D* vertices4 = new Point4D[24];
    vertices4[0] = Point4D (0 , 0, 0.25 , 0.25);
    vertices4[1] = Point4D(-_width, _width, 0, 0.5);
    vertices4[2] = Point4D(-_width, -_width, 0,0);
    vertices4[3] = Point4D(_width, -_width , 0.5,0);
    vertices4[4] = Point4D(_width, _width, 0.5 ,0.5);
    vertices4[5] = vertices4[1];

    vertices4[6] = Point4D (0 , 0, 0.75 , 0.25);
    vertices4[7] = Point4D(-_width, _width, 0.5, 0.5);
    vertices4[8] = Point4D(-_width, -_width, 0.5,0);
    vertices4[9] = Point4D(_width, -_width , 1.0,0);
    vertices4[10] = Point4D(_width, _width, 1.0 ,0.5);
    vertices4[11] = vertices4[7];

    vertices4[12] = Point4D (0 , 0, 0.25 , 0.75);
    vertices4[13] = Point4D(-_width, _width, 0, 1.0);
    vertices4[14] = Point4D(-_width, -_width, 0,0.5);
    vertices4[15] = Point4D(_width, -_width , 0.5,0.5);
    vertices4[16] = Point4D(_width, _width, 0.5 ,1.0);
    vertices4[17] = vertices4[13];

    vertices4[18] = Point4D (0 , 0, 0.75 , 0.75);
    vertices4[19] = Point4D(-_width, _width, 0.5, 1.0);
    vertices4[20] = Point4D(-_width, -_width, 0.5,0.5);
    vertices4[21] = Point4D(_width, -_width , 1.0,0.5);
    vertices4[22] = Point4D(_width, _width, 1 ,1);
    vertices4[23] = vertices4[19];

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(Point4D), vertices4, GL_STATIC_DRAW);
    delete[] vertices4;

}
void Patrol::draw()
{
    if (_firstDraw)
    {
        _beginTime = currTime();
        _firstDraw = false;
    }
//    int lastphase = phase;
    long long ct = currTime() ;
    long long dt = ct - _beginTime;
    long long phase = (1.0*dt / 1000.0)*4.0 ;
    phase = phase % 4;
//    if (lastphase !=phase)
//        LOGD ("phase=%d", (int) phase);
    offset = (void*) (phase * 24*4);
    drawTexture();
	return;
}

bool Patrol::out() const
{
	return x< view->left()-0.2 || x > view->right() + 0.2;
}

void Patrol::moveStep(float delta)
{
    FlyingObject::moveStep(delta);
	float dw = 0.1;
	if (!leftShoot)
		if (x >= view->left() && x <= view->left() + dw)
		{
			view->patrolShoot(this);
			leftShoot = true;
		}
	if (!rightShoot)
		if (x >= view->right() - dw && x <= view->right())
		{
			view->patrolShoot(this);
			rightShoot = true;
		}
}

void Patrol::getCurrentCoords(Point *_vertices, int *_nvertices) const
{
	*_nvertices = 4;
	_vertices[0] = Point (-width +x, _r+y, 0);
	_vertices[1] = Point (x+width, _r+y, 0);
	_vertices[2] = Point (width +x, y-_r, 0);
	_vertices[3] = Point (x-width, -_r + y, 0);
}
