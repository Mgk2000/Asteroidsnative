#ifndef SHIP_H
#define SHIP_H
#include "flyingobject.h"
class View;
class Ship : public FlyingObject
{
public:
	Ship(View* view);
	virtual ~Ship();
	void init();
	void initGL();
	void setX(float _x);
	bool touched(float _x, float _y) const;
	float top() const;
	virtual void getCurrentCoords (Point * vertices, int* nvertices ) const;
	virtual void moveStep();

	void die();
	void revive();
	bool dead() {return _dead;}
	const Point4D&  color() const;
	void draw();
    float height() const {return _height;}

private:
    float _width;
    float _height;
	bool _dead;
	Point4D alivecolor, deadcolor;
};

#endif // SHIP_H
