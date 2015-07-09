#ifndef ROUNDEDDRECTANGLE_H
#define ROUNDEDDRECTANGLE_H
#include "flyingobject.h"
class RoundedRectangle : public FlyingObject
{
public:
    RoundedRectangle(View* view);
    void init();
    void initGL();
};

#endif // ROUNDEDDRECTANGLE_H
