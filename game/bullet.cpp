#include "bullet.h"
#include <math.h>
#include "view.h"

#include "logmsg.h"
Bullet::Bullet(View* view, float _x, float _y, float _angle)
    : FlyingObject(view, 1, _x, _y, 1.0f, _angle)
{
}

Bullet::~Bullet()
{
}

void Bullet::init()
{
	vertices = new Point[2];
    float length = 0.05f;
    vertices[1] = Point (0 , 0);
    vertices[0] = Point (length * sin(angle), length * cos (angle));
	nvertices = 2;
	_color = Point4D (0.9, 0.9, 0.0, 1.0);
    topdx = length * sin(angle);
    topdy = length * cos(angle);
	FlyingObject::init();
	initGL();

}
void Bullet::initGL()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(Point), vertices, GL_STATIC_DRAW);
}

void Bullet::draw()
{
    drawLines(GL_LINE_STRIP, vboIds[0], 2, color(), 4.0);
}

Point Bullet::top() const
{
    return Point (x + topdx, y + topdy ,0);
}
Point Bullet::bottom() const
{
	return Point (x ,y ,0);
}

bool Bullet::out() const
{
    return x <view->left() || x > view->right() || y > view->top() || y < view->bottom();
}
