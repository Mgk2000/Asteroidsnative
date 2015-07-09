#include "roundedrectangle.h"
#include "points.h"
#include <math.h>
RoundedRectangle::RoundedRectangle(View* view) : FlyingObject (view, 3, 0)
{
}

void RoundedRectangle::init()
{
    float r = 0.25;
    int nr = 5;
    nvertices = nr * 4;
    vertices = new Point(nvertices);
    Point c (-1 + r, 1-r);
    float fi =0;
    float dfi = -  M_PI / 2.0 /(nr-1);
    for (int i =0; i< vertices; i++)
    {
        vertices[i] = Point( c.x + r* sin(fi), c.y + r * sin(fi));
        fi = fi + dfi;
        if (i == nr)
            c = Point(-1+r, -1+r);
        else if (i == nr*2)
            c = Point(1-r, -1+r);
        else if (i == nr* 3)
            c= Point (1-r, 1 -r);
    }
    initGL();
}
