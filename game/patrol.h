#ifndef PATROL_H
#define PATROL_H
#include "flyingobject.h"

class View;
class Random;
class Patrol : public FlyingObject
{
public:
    Patrol(View* _view, Texture* texture);
	virtual ~Patrol();
	void init();
	void initGL();
	void draw();
	bool out() const;
    void moveStep(float delta);
	void getCurrentCoords (Point * vertices, int* nvertices ) const;
    virtual int cost() const {return 20;}

private:
	float width;
	bool leftShoot, rightShoot;
    long long _beginTime;
    bool _firstDraw;

};

#endif // PATROL_H
