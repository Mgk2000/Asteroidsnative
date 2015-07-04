#ifndef TARGET_H
#define TARGET_H
#include "asteroid.h"
class Target : public Asteroid
{
public:
    Target(View* __view, Texture* __texture,
           float _x, float _y, float _radius);
    void initParams();
    void applyParams();
    void draw();
private:
};

#endif // TARGET_H
