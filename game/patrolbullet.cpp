#include "patrolbullet.h"


PatrolBullet::PatrolBullet(View *_view, float _x, float _y, float _angle) : Bullet (_view,  _x,  _y,  _angle)
{

}

void PatrolBullet::init()
{
	Bullet::init();
	speed = speed*0.5;
	FlyingObject::init();
	_color = Point4D (1, 0, 0, 1);
}
