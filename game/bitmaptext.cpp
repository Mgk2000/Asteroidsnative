#include "bitmaptext.h"
#include "texture.h"
#include "bonus.h"

Letter::Letter(View* view, Texture* texture ) : FlyingObject (view , 1,texture )
{

}

void Letter::init(int nrows)
{
    nvertices = 4;
    vertices = new Point[nvertices];
    this->_r = 1.0;
    vertices[0] = Point(-_r, _r);
    vertices[1] = Point(-_r, -_r);
    vertices[2] = Point(_r, -_r);
    vertices[3] = Point(_r, _r);

    initGL(nrows);

}

void Letter::initGL(int nrows)
{
    int x = index % 8;
    int y = index / 8;
    y = nrows -1 -y;
//    int y = nrows -(index+7) / 8;
    float x0 = 1.0 * x / 8;
    float y0 = 1.0 * y / nrows;
    float x1 = 1.0 * (x + 1) / 8;
    float y1 = 1.0 * (y + 1) / nrows;
    Point4D* vertices4 = new Point4D[6];
    vertices4[0] = Point4D (0 , 0, (x0+ x1) * 0.5 , (y0+y1) * 0.5);
    vertices4[1] = Point4D(vertices[0].x, vertices[0].y, x0,y1);
    vertices4[2] = Point4D(vertices[1].x, vertices[1].y, x0,y0);
    vertices4[3] = Point4D(vertices[2].x, vertices[2].y, x1,y0);
    vertices4[4] = Point4D(vertices[3].x, vertices[3].y, x1, y1);
    vertices4[5] = vertices4[1];
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, (nvertices+2) * sizeof(Point4D), vertices4, GL_STATIC_DRAW);
    delete[] vertices4;

}

BitmapText::BitmapText(View* view, Texture* texture)
{
    cellSize = texture->picWidth() /8;
    this->nrows = texture->picHeight() / cellSize;
    Letter* l;
    l = new Letter (view, texture);
    l->index = 0;
    l->width = 90;
    letters['A'] = l;
    l = new Letter (view, texture);
    l->index = 1;
    l->width = 77;
    letters['B'] = l;
    l = new Letter (view, texture);
    l->index = 2;
    l->width = 80;
    letters['C'] = l;
    l = new Letter (view, texture);
    l->index = 3;
    l->width = 87;
    letters['D'] = l;
    l = new Letter (view, texture);
    l->index = 4;
    l->width = 74;
    letters['E'] = l;
    l = new Letter (view, texture);
    l->index = 5;
    l->width = 65;
    letters['F'] = l;
    l = new Letter (view, texture);
    l->index = 6;
    l->width = 90;
    letters['G'] = l;
    l = new Letter (view, texture);
    l->index = 7;
    l->width = 89;
    letters['H'] = l;
    l = new Letter (view, texture);
    l->index = 8;
    l->width = 39;
    letters['I'] = l;
    l = new Letter (view, texture);
    l->index = 9;
    l->width = 48;
    letters['J'] = l;
    l = new Letter (view, texture);
    l->index = 10;
    l->width = 93;
    letters['K'] = l;
    l = new Letter (view, texture);
    l->index = 11;
    l->width = 74;
    letters['L'] = l;
    l = new Letter (view, texture);
    l->index = 12;
    l->width = 127;
    letters['M'] = l;
    l = new Letter (view, texture);
    l->index = 13;
    l->width = 90;
    letters['N'] = l;
    l = new Letter (view, texture);
    l->index = 14;
    l->width = 86;
    letters['O'] = l;
    l = new Letter (view, texture);
    l->index = 15;
    l->width = 66;
    letters['P'] = l;
    l = new Letter (view, texture);
    l->index = 16;
    l->width = 87;
    letters['Q'] = l;
    l = new Letter (view, texture);
    l->index = 17;
    l->width = 86;
    letters['R'] = l;
    l = new Letter (view, texture);
    l->index = 18;
    l->width = 63;
    letters['S'] = l;
    l = new Letter (view, texture);
    l->index = 19;
    l->width = 74;
    letters['T'] = l;
    l = new Letter (view, texture);
    l->index = 20;
    l->width = 90;
    letters['U'] = l;
    l = new Letter (view, texture);
    l->index = 21;
    l->width = 90;
    letters['V'] = l;
    l = new Letter (view, texture);
    l->index = 22;
    l->width = 119;
    letters['W'] = l;
    l = new Letter (view, texture);
    l->index = 23;
    l->width = 90;
    letters['X'] = l;
    l = new Letter (view, texture);
    l->index = 24;
    l->width = 90;
    letters['Y'] = l;
    l = new Letter (view, texture);
    l->index = 25;
    l->width = 73;
    letters['Z'] = l;
    l = new Letter (view, texture);
    l->index = 26;
    l->width = 56;
    letters['a'] = l;
    l = new Letter (view, texture);
    l->index = 27;
    l->width = 59;
    letters['b'] = l;
    l = new Letter (view, texture);
    l->index = 28;
    l->width = 50;
    letters['c'] = l;
    l = new Letter (view, texture);
    l->index = 29;
    l->width = 63;
    letters['d'] = l;
    l = new Letter (view, texture);
    l->index = 30;
    l->width = 55;
    letters['e'] = l;
    l = new Letter (view, texture);
    l->index = 31;
    l->width = 55;
    letters['f'] = l;
    l = new Letter (view, texture);
    l->index = 32;
    l->width = 61;
    letters['g'] = l;
    l = new Letter (view, texture);
    l->index = 33;
    l->width = 62;
    letters['h'] = l;
    l = new Letter (view, texture);
    l->index = 34;
    l->width = 30;
    letters['i'] = l;
    l = new Letter (view, texture);
    l->index = 35;
    l->width = 37;
    letters['j'] = l;
    l = new Letter (view, texture);
    l->index = 36;
    l->width = 64;
    letters['k'] = l;
    l = new Letter (view, texture);
    l->index = 37;
    l->width = 37;
    letters['l'] = l;
    l = new Letter (view, texture);
    l->index = 38;
    l->width = 98;
    letters['m'] = l;
    l = new Letter (view, texture);
    l->index = 39;
    l->width = 62;
    letters['n'] = l;
    l = new Letter (view, texture);
    l->index = 40;
    l->width = 60;
    letters['o'] = l;
    l = new Letter (view, texture);
    l->index = 41;
    l->width = 59;
    letters['p'] = l;
    l = new Letter (view, texture);
    l->index = 42;
    l->width = 59;
    letters['q'] = l;
    l = new Letter (view, texture);
    l->index = 43;
    l->width = 42;
    letters['r'] = l;
    l = new Letter (view, texture);
    l->index = 44;
    l->width = 44;
    letters['s'] = l;
    l = new Letter (view, texture);
    l->index = 45;
    l->width = 34;
    letters['t'] = l;
    l = new Letter (view, texture);
    l->index = 46;
    l->width = 63;
    letters['u'] = l;
    l = new Letter (view, texture);
    l->index = 47;
    l->width = 62;
    letters['v'] = l;
    l = new Letter (view, texture);
    l->index = 48;
    l->width = 90;
    letters['w'] = l;
    l = new Letter (view, texture);
    l->index = 49;
    l->width = 61;
    letters['x'] = l;
    l = new Letter (view, texture);
    l->index = 50;
    l->width = 63;
    letters['y'] = l;
    l = new Letter (view, texture);
    l->index = 51;
    l->width = 54;
    letters['z'] = l;
    l = new Letter (view, texture);
    l->index = 52;
    l->width = 58;
    letters['0'] = l;
    l = new Letter (view, texture);
    l->index = 53;
    l->width = 47;
    letters['1'] = l;
    l = new Letter (view, texture);
    l->index = 54;
    l->width = 57;
    letters['2'] = l;
    l = new Letter (view, texture);
    l->index = 55;
    l->width = 52;
    letters['3'] = l;
    l = new Letter (view, texture);
    l->index = 56;
    l->width = 64;
    letters['4'] = l;
    l = new Letter (view, texture);
    l->index = 57;
    l->width = 54;
    letters['5'] = l;
    l = new Letter (view, texture);
    l->index = 58;
    l->width = 58;
    letters['6'] = l;
    l = new Letter (view, texture);
    l->index = 59;
    l->width = 57;
    letters['7'] = l;
    l = new Letter (view, texture);
    l->index = 60;
    l->width = 56;
    letters['8'] = l;
    l = new Letter (view, texture);
    l->index = 61;
    l->width = 57;
    letters['9'] = l;
    l = new Letter (view, texture);
    l->index = 62;
    l->width = 22;
    letters['.'] = l;
    l = new Letter (view, texture);
    l->index = 63;
    l->width = 29;
    letters[','] = l;
    l = new Letter (view, texture);
    l->index = 64;
    l->width = 24;
    letters[':'] = l;
    l = new Letter (view, texture);
    l->index = 65;
    l->width = 37;
    letters['-'] = l;
    l = new Letter (view, texture);
    l->index = 66;
    l->width = 69;
    letters['+'] = l;
    l = new Letter (view, texture);
    l->index = 67;
    l->width = 39;
    letters['('] = l;
    l = new Letter (view, texture);
    l->index = 68;
    l->width = 39;
    letters[')'] = l;
    l = new Letter (view, texture);
    l->index = 69;
    l->width = 27;
    letters['!'] = l;
    l = new Letter (view, texture);
    l->index = 70;
    l->width = 51;
    letters['?'] = l;
    bonus = new Bonus(view, Bonus::NONE,0);

}

void BitmapText::init()
{
    std::map<char, Letter*>::iterator it = letters.begin();
    for (; it!= letters.end(); it++)
        it->second->init(nrows);
    bonus->init();
}

void BitmapText::initGL()
{
    std::map<char, Letter*>::iterator it = letters.begin();
    for (; it!= letters.end(); it++)
        it->second->initGL(nrows);
}
void BitmapText::draw(float x, float y, float scale, Point4D color,float lwidth,  const char *txt)
{
    float left = x;
    for (unsigned int i=0; i< strlen(txt); i++)
    {
        char c = txt[i];
        if (c=='%')
        {
            char buf[4] = {0};
            int j = i+1;
            for (; txt[j] != '%'; j++)
                buf[j-i-1] = txt[j];
            int n;
            sscanf(buf, "%d",  &n);
            bonus->setKind((Bonus::Kind) n);
            bonus->setScale(scale);
            bonus->setX(left);
            bonus->setY(y);
            bonus->draw();
            i = j;
            continue;
        }
        Letter *l = letters[c];
        if (!l)
        {
            left = left + 1.0 * scale;
            continue;
        }
        l->setScale (scale);
        l->setX(left);
        l->setY(y);
        l->setColorMult(color);
        l->drawTexture();
        left = left + (0.0 + 2.0 * l->width/cellSize) * scale; //1.0* l->width/cellSize * scale;
    }
}
void BitmapText::drawCenter(float x, float y, float scale, Point4D color, float lwidth, const char *txt)
{
    float w = textWidth(txt)* scale;
    draw(x- w /2, y, scale, color, lwidth, txt);
}

float BitmapText::textWidth(const char *txt)
{
    float w = -2.0;
    for (unsigned int i=0; i< strlen(txt); i++)
    {
        char c = txt[i];
        Letter *l = letters[c];
        if (!l)
            w = w + 1.0;
        else
            w = w + l->width;
    }
    return w;
}