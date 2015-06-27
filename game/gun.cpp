#include "gun.h"
#include <math.h>
#include "view.h"
#include "math_helper.h"
#include "ship.h"

Gun::Gun(View* _view) :  FlyingObject(_view, 1)
{
	init ();
}

Gun::~Gun()
{
}
#define NP 129
void Gun::initGL()
{
	init();
}
void Gun::init()
{
	Point points[NP];
	float w = 0.6;
	float h = 0.0;
	y = - 1 - h;
    _r = sqrt(h * h + w * w)* 0.7;
	double fi0 = atan2(h, w);
    double fi1 = M_PI*2 - 2* fi0;
	for (int i =0; i< NP; i++)
	{
		double fi = fi0 + fi1 * i / (NP-1);
		points [i] = Point (_r * cos(fi) , _r * sin(fi), 0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
	glBufferData(GL_ARRAY_BUFFER, NP * sizeof(Point), points, GL_STATIC_DRAW);
	_color = Point4D (0.5, 0.5, 0.0, 1.0);
}

bool Gun::touched(float _x, float _y, float* fi) const
{
	const float delta = 0.15;
    float _rr = sqrt(sqr(_x-x) + sqr (_y-y));
    float dr = _r-_rr;
	bool b = dr < delta && dr > -delta;
	if (b)
	{
        *fi = (float) atan2(_x-x,_y-y);
	}
	return b;
}

void Gun::draw()
{
    x=view->getShip()->X();
    y=view->getShip()->Y() + view->getShip()->height();
	drawLines(GL_LINE_STRIP, vboIds[0], NP, color(), 4.0);
}

