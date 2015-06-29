#ifndef BACKGROUND_H
#define BACKGROUND_H
#include "flyingobject.h"
class Texture;
class View;

class Background : public FlyingObject
{
public:
    Background(View* _view, Texture * _texture);
    void init();
    void draw();
    Point4D points[6];
};

#endif // BACKGROUND_H
