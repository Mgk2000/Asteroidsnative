#include "explosion.h"
#include "view.h"
#include "ship.h"
#include <math.h>
#define NP 129
Explosion::Explosion (View* _view, float __radius) : FlyingObject(_view, 1, 0), _radius(__radius)
{
    x = _view->getShip()->X();
    y = _view->getShip()->Y() + view->getShip()->height();
    vx = 0; vy = 0;
    nvertices = NP;
    vertices = new Point[nvertices];
    for (int i=0; i< nvertices; i++)
    {
        float fi = M_PI* 2* i/ (nvertices-1);
        vertices[i] = Point(_radius* sin(fi), _radius* cos(fi));
    }
    _color = Point4D(0.5,0,0,1);
    _endTime = currTime() + 1500;
    initGL();
}

void Explosion::initGL()
{
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(Point3D), vertices, GL_STATIC_DRAW);
}

void Explosion::draw()
{
   setScale(1.0* (currTime()-_startTime) / (_endTime-_startTime));
    drawLines(GL_LINE_STRIP, vboIds[0], NP, color(), 12.0);
}
bool Explosion::out() const
{
    return currTime() > _endTime;
}
