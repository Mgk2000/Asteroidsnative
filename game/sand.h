#ifndef SAND_H
#define SAND_H
#include "flyingobject.h"
class View;
class Sand : public FlyingObject
{
public:
    Sand(View* _view, FlyingObject* parent, const  Color& col = COLOR_WHITE);
    void initGL();
    bool out() const;
    void draw();
private:
    long long _endTime;
};

#endif // SAND_H
