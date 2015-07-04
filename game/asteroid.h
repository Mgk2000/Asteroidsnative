#ifndef ASTEROID_H
#define ASTEROID_H
#include "flyingobject.h"
class View;
class Random;
class Texture;
class Bonus;
class Asteroid : public FlyingObject
{
public:
    Asteroid(View * _view, Texture * __texture);
	virtual ~Asteroid();
	void init();
    virtual void initGL();
    virtual void initParams();
    virtual void applyParams();
	void draw();
	bool isPointInside( Point* p) const;
	bool out() const;
    void moveStep(float delta);
    inline virtual bool isSplinter() const {return false;}
	float R() const {return _rr;}
	void getCurrentCoords (Point * vertices, int* nvertices ) const;
    inline virtual int cost() const {return 10;}
    Bonus* bonus() const {return _bonus;}
    void setBonus(Bonus* __bonus) {_bonus = __bonus;}
protected:
	Point* rotatedVertices;
	float _rr;
	Random* random;
    float texCenterX, texCenterY;
    float texScale;
    Bonus* _bonus;
};

class Splinter : public Asteroid
{
public:
    Splinter (View* view, Texture * __texture);
	virtual ~Splinter();
	void init(const Asteroid &parent, float fi);
    inline bool isSplinter() const {return true;}
    inline virtual int cost() const {return 5;}
};

#endif // ASTEROID_H
