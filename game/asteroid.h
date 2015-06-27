#ifndef ASTEROID_H
#define ASTEROID_H
#include "flyingobject.h"
class View;
class Random;
class Asteroid : public FlyingObject
{
public:
    Asteroid(View * _view);
	virtual ~Asteroid();
	void init();
	void initGL();
	void initParams();
	void applyParams();
	void draw();
	bool isPointInside( Point* p) const;
	bool out() const;
    void moveStep(float delta);
    inline virtual bool isSplinter() const {return false;}
	float R() const {return _rr;}
	void getCurrentCoords (Point * vertices, int* nvertices ) const;
    inline virtual int cost() const {return 10;}

protected:
	Point* rotatedVertices;
	float _rr;
	Random* random;
};

class Splinter : public Asteroid
{
public:
    Splinter (View* view);
	virtual ~Splinter();
	void init(const Asteroid &parent, float fi);
    inline bool isSplinter() const {return true;}
    inline virtual int cost() const {return 5;}
};

#endif // ASTEROID_H
