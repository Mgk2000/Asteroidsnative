#ifndef GUN_H
#define GUN_H
#include "flyingobject.h"

class View;

class Gun : public FlyingObject
{
public:
	Gun(View* );
    virtual ~Gun();
	void draw();
	void init();
	void initGL();
	bool touched(float _x, float _y, float* fi) const;
private:
};

#endif // GUN_H
