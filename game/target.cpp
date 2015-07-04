#include "target.h"
#include <math.h>
#include "text.h"
#include "bonus.h"
#define NP 16
Target::Target(View* __view, Texture* __texture,
    float _x, float _y, float _radius) : Asteroid(__view, __texture)

{
    x = _x;
    y = _y;
    _r= _radius;
    vx = 0;
    vy = 0;
    _colorMult = Point4D(0, 2, 2);
}

void Target::initParams()
{
    _rr = _r;
    nvertices = NP;
    vertices = new Point[nvertices];
    rotatedVertices = new Point[nvertices];
    texCenterX = 0.5;
    texCenterY = 0.5;
    texScale = 1.0;

}
void Target::applyParams()
{
    for (int i=0; i< nvertices; i++)
    {
        float fi = M_PI*2 * i /nvertices;
        vertices[i] = Point (_r * sin(fi) , _r * cos(fi));
        rotatedVertices[i] = vertices[i];
    }
    initGL();
}

void Target::draw()
{
    if (broken())
        return;
    if (_bonus)
    {
        _bonus->setX(x);
        _bonus->setY(y -0.05);
    }
//    FlyingObject::draw();
    drawTexture();

    char buf[6];
    sprintf(buf, "%d", _breakCount - _shootCount);
    text()->drawCenter(x, y+0.05, 0.025, Point4D(1,1,0,1), 3, buf);
}
