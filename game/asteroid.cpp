#include "asteroid.h"
#include <math.h>
#include "view.h"
#include "math_helper.h"
#include "random.h"
#include "ship.h"

Asteroid::Asteroid(View * _view, Texture * __texture) :
    FlyingObject (_view, 1, __texture)
{
}

Asteroid::~Asteroid()
{
	delete[] rotatedVertices;
}

void Asteroid::init()
{
	initParams();
	applyParams();
}

void Asteroid::initParams()
{
	x = random1().frandom(-1.0, 1.0);
    _rr = random1().frandom(0.06, 0.10);
    y = 1.+0.5* _rr;
    float _x = view->getShip()->X();
    float _y = view->getShip()->Y();
    angle = atan2(_x - x, _y - y);
    speed = 0.25;
    vx = speed* sin (angle);
    _rotateSpeed = random1().frandom(-100.1, 100.1);
	vy = speed* cos (angle);
    nvertices = random1().irandom(MAXVERTICES-20, MAXVERTICES);
    _colorMult.r = random1().frandom (0.0, 2.0);
    _colorMult.g = random1().frandom (0.0, 2.0);
    _colorMult.b = random1().frandom (0.0, 2.0);
}

void Asteroid::applyParams()
{
//	vertices = new Point[nvertices];
//	rotatedVertices = new Point[nvertices];
    vertices = new Point[nvertices];
    rotatedVertices = new Point[nvertices];
    _r = 0.0f;
/*	for (int i=0; i< nvertices; i++)
	{
		float fi = M_PI*2 * i /nvertices;
		fi = fi + random1().frandom(-M_PI / nvertices /2., M_PI / nvertices /2);
        float r1 = _rr * random1().frandom(0.9, 1.1);
        if (r1> _r)
            _r = r1;
		vertices[i] = Point (r1 * sin(fi) , r1 * cos(fi), 0);
		rotatedVertices[i] = vertices[i];
    }
	_color = Point4D (0.3 + random1().frandom()*0.7 , 0.3 + random1().frandom()*0.7, 0.3 + random1().frandom()*0.7, 1.0);
*/
    texScale = 2.0;
    float sr = _rr  * texScale;
    texCenterX = random1().frandom( sr, _texture->picWidth()-sr);
    texCenterY = random1().frandom( sr, _texture->picHeight() - sr);
    for (int i=0; i< nvertices; i++)
    {
        float fi = M_PI*2 * i /nvertices;
        fi = fi + random1().frandom(-M_PI / (nvertices) /2., M_PI / nvertices /2);
        float r1 = _rr * random1().frandom(0.9, 1.1);
        if (r1> _r)
            _r = r1;
        sr = r1 * texScale;
        vertices[i] = Point (r1 * sin(fi) , r1 * cos(fi));
        rotatedVertices[i] = vertices[i];
    }
    initGL();
}
void Asteroid::initGL()
{
    Point4D* vertices4 = new Point4D[nvertices+2];
    vertices4[0] = Point4D (0 , 0, texCenterX , texCenterY);
    for (int i =0; i< nvertices; i++)
        vertices4[i+1] = Point4D(vertices[i].x, vertices[i].y,
            texCenterX + vertices[i].x * texScale , texCenterY + vertices[i].y * texScale);
    vertices4[nvertices+1] = vertices4[1];
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, (nvertices+2) * sizeof(Point4D), vertices4, GL_STATIC_DRAW);
    delete[] vertices4;
}

void Asteroid::draw()
{
//	drawLines(GL_LINE_LOOP,vboIds[0],nvertices,color(), 5.0);
    drawTexture();
}

bool Asteroid::isPointInside(Point *p) const
{
    Point center(x,y);
	return ::isInside(p, rotatedVertices, &center, nvertices, true);
}

bool Asteroid::out() const
{
    return y <  view->bottom();
}

void Asteroid::moveStep(float delta)
{
    FlyingObject::moveStep(delta);
    rotateAngle = rotateAngle + _rotateSpeed * delta;
    rotatePoints(vertices, rotatedVertices, rotateAngle , nvertices);
}

void Asteroid::getCurrentCoords(Point *_vertices, int *_nvertices) const
{
	*_nvertices = nvertices;
	for (int i=0; i< nvertices; i++)
	{
		_vertices[i].x = rotatedVertices[i].x+x;
		_vertices[i].y = rotatedVertices[i].y+y;
	}
}

Splinter::Splinter(View *view, Texture * __texture) : Asteroid (view, __texture)
{
}

Splinter::~Splinter()
{
}

void Splinter::init(const Asteroid &parent, float fi)
{
    _rr = parent.R() /2.;
	x = parent.X() + parent.R() * cos(fi);
	y = parent.Y() + parent.R() * sin(fi);
    _rotateSpeed = parent.rotateSpeed();
    float dv = 0.05;
	vx = parent.VX() + dv *  cos(fi);
    vy = parent.VY() + dv *  sin(fi);
	_color = parent.color();
	angle = atan2(vx, vy);
    nvertices = random2().irandom(4,8);
    _colorMult = parent.colorMult();
    applyParams();
}

void Splinter::applyParams1()
{
    vertices = new Point[nvertices];
    rotatedVertices = new Point[nvertices];
    _r = 0.0f;
    for (int i=0; i< nvertices; i++)
    {
        float fi = M_PI*2 * i /nvertices;
        fi = fi + random1().frandom(-M_PI / nvertices /2., M_PI / nvertices /2);
        float r1 = _rr * random1().frandom(0.9, 1.1);
        if (r1> _r)
            _r = r1;
        vertices[i] = Point (r1 * sin(fi) , r1 * cos(fi), 0);
        rotatedVertices[i] = vertices[i];
    }
    _color = Point4D (0.3 + random1().frandom()*0.7 , 0.3 + random1().frandom()*0.7, 0.3 + random1().frandom()*0.7, 1.0);
    initGL();

}

void Splinter::draw1()
{
    drawLines(GL_LINE_LOOP,vboIds[0],nvertices,color(), 5.0);
}

void Splinter::initGL1()
{
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(Point4D), vertices, GL_STATIC_DRAW);

}

