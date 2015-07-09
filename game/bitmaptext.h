#ifndef BITMAPTEXT_H
#define BITMAPTEXT_H
#include <map>
#include "flyingobject.h"
class Letter : public FlyingObject
{
protected:
    Letter(View* view, Texture* texture);
    float width;
    int index;
    void init(int nrows);
    void initGL(int nrows);
    friend class BitmapText;
};
class View;
class Texture;
class Bonus;
class BitmapText
{
public:
    BitmapText(View* view, Texture* texture);
    void init();
    void draw(float x, float y, float scale, Point4D color, float lwidth, const char *txt);
    void drawCenter(float x, float y, float scale, Point4D color, float lwidth, const char *txt);
    void initGL();
    float textWidth(const char* txt);
private:
    std::map <char, Letter*> letters;
    Bonus* bonus;
    int cellSize;
    int nrows;
};

#endif // BITMAPTEXT_H
