#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "flyingobject.h"
class Explosion : public FlyingObject
{
public:
    Explosion(View* view, float __radius);
    bool out() const;
    void draw();
private:
    float _radius;
    long long _endTime;
    void initGL();
};

#endif // EXPLOSION_H
