#ifndef TEXT_H
#define TEXT_H
#include "flyingobject.h"

#include <map>
class View;

class Letter : public FlyingObject
{
public:
    Letter() : FlyingObject (0,1){}
    Letter(View* view, char c);
    float width;
    void draw();
    void initGL();
    float linewidth;
private:
    void mkPoints(int np);
    void init(char c);
    void init_0();
    void init_1();
    void init_2();
    void init_3();
    void init_4();
    void init_5();
    void init_6();
    void init_7();
    void init_8();
    void init_9();
    void init_S();
    void init_L();
    void init_E();
    void init_N();
    void init_D();
    void init_semicolon();
    int what;
};

class Text
{
public:
    Text(View* view);
    ~Text();
    void draw(float x, float y, float scale, Point4D color, float lwidth, const char *txt);
    void initGL();
private:
    std::map<char, Letter* > letters;
};

#endif // TEXT_H
