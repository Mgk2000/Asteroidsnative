#include "sand.h"
#include "random.h"
#include <math.h>

Sand::Sand(View* _view, FlyingObject* _parent) : FlyingObject(_view, 1, 0)
{
    float pr = _parent->r();
    x = _parent->X();
    y = _parent->Y();
    vx = _parent->VX();
    vy = _parent->VY();
    _startTime = currTime();
    _endTime = _startTime+ 1500;
    nvertices = random2().irandom(20, 30)* 2;
    vertices = new Point[nvertices];
    for (int i = 0; i< nvertices/2; i+=2)
    {
        float rr = pr*random2().frandom();
        double fi = random2().frandom(0.0, M_PI*2);
        float dx = rr * sin(fi);
        float dy = rr * cos(fi);
        vertices[i*2].x =  dx;
        vertices[i*2].y = dy;
        vertices[i*2+1].x = vertices[i*2].x;
        vertices[i*2+1].y = vertices[i*2].y+ 0.003;
    }
    _color = Point4D (1,1,1,1); //_parent->color();
    initGL();
}

void Sand::initGL()
{
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(Point3D), vertices, GL_STATIC_DRAW);
}

bool Sand::out() const
{
    return currTime() > _endTime;
}

void Sand::draw()
{
    float lwidth = 2.0 * (_endTime-currTime()+1000) / 1500;
    drawLines(GL_LINES, vboIds[0] , nvertices,_color,lwidth);
}
