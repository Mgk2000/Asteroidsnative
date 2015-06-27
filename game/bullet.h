#ifndef BULLET_H
#define BULLET_H
#include "flyingobject.h"

class View;

class Bullet : public FlyingObject
{
public:
	Bullet(View* view, float _x, float _y, float _angle);
	virtual ~Bullet();
	void init();
	void initGL();
	void draw();
	Point top() const;
	Point bottom() const;
	bool out() const;
	virtual bool isMy() const {return true;}
private:
    float topdx, topdy;
};

#endif // BULLET_H
