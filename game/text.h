#ifndef TEXT_H
#define TEXT_H
#include "flyingobject.h"

#include <map>
class View;
class Text
{
public:
    Text(View* view);
    ~Text();
    void draw(float x, float y, float scale, const Point4D &color, float lwidth, const char *txt);
    void drawCenter(float x, float y, float scale, const Point4D & color, float lwidth, const char *txt);
    void initGL();
    float textWidth(const char* txt);
private:
    class Letter : public FlyingObject
    {
    public:
        Letter() : FlyingObject (0,1,0){}
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

        void init_D();
        void init_E();
        void init_K();
        void init_L();
        void init_N();
        void init_O();
        void init_S();
        void init_Y();

        void init_a();
        void init_c();
        void init_d();
        void init_e();
        void init_i();
        void init_k();
        void init_l();
        void init_n();
        void init_o();
        void init_r();
        void init_s();
        void init_t();
        void init_u();
        void init_v();

        void init_exclaim();
        void init_semicolon();
        void init_point();
        void init_comma();
        void init_minus();
        int what;
    };
    std::map<char, Letter* > letters;
};

#endif // TEXT_H
