#ifndef FLYINGOBJECT_H
#define FLYINGOBJECT_H
#include <globject.h>
#include "points.h"
class Random;
class View;
class Mat4;
#define MAXVERTICES 60
class FlyingObject : public GlObject
{
public:
	FlyingObject(View* _view, int _nbos);
	FlyingObject(View* _view, int _nbos, float _x, float _y, float _speed, float _angle);
	virtual ~FlyingObject();
	virtual void init();
	virtual void draw();
    virtual void moveStep(float delta);
	float X() const {return x;}
    void setX(float _x){x=_x;}
	float Y() const {return y;}
    void setY(float _y){y=_y;}
    float VX() const {return vx;}
	float VY() const {return vy;}
    inline void setColor(float r , float g , float b) {_color = Point4D (r,g,b,1.0);}
    inline void setColor( const Point4D&  __color) {_color = __color;}
    inline virtual const Point4D&  color() const {return _color;}
	virtual void getCurrentCoords (Point * _vertices, int* _nvertices ) const;
	bool isIntersects(const FlyingObject& obj)  const;
	virtual bool isPointInside( Point* p) const;
    inline void setScale (float sc) {_scale = sc;}
    inline virtual int cost() const {return 0;}
    inline float r() const {return _r;}
    inline float rotateSpeed() const {return _rotateSpeed;}
protected:
	float x,y,angle;
    float rotateAngle;
	uint* vboIds;
	int nvbos;
	Point* vertices;
	int nvertices;
	GLushort* indices;
	int nindices;
	Point4D _color;
	void fill_vbos();
	View* view;
    float _rotateSpeed;
	float speed;
	float vx, vy;
    float _r;
	Random &random1() const;
	Random &random2() const;
	void drawTriangles();
	void drawLines(int how, uint vbo, int npoints, const Point4D& color, float _width, float angle = 0.0);
    float _scale;
};


#endif // FLYINGOBJECT_H
