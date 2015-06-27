#ifndef PATROLBULLET_H
#define PATROLBULLET_H
#include "bullet.h"
class PatrolBullet : public Bullet
{
public:
	PatrolBullet(View* view, float _x, float _y, float _angle);
	void init();
	virtual bool isMy() const {return false;}

};

#endif // PATROLBULLET_H
