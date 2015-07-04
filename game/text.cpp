#include "text.h"

Letter::Letter(View* view, char c) : FlyingObject (view, 1,0),
    width(1.8), what(GL_LINE_STRIP), linewidth(2.0)
{
    init (c);
}

void Letter::draw()
{
    if (nvertices)
        drawLines(what , vboIds[0], nvertices, color(), linewidth);
}


void Letter::mkPoints(int np)
{
    nvertices = np;
    vertices = new Point[np];
}

void Letter::init(char c)
{
    switch (c)
    {
    case '0': init_0(); break;
    case '1': init_1(); break;
    case '2': init_2(); break;
    case '3': init_3(); break;
    case '4': init_4(); break;
    case '5': init_5(); break;
    case '6': init_6(); break;
    case '7': init_7(); break;
    case '8': init_8(); break;
    case '9': init_9(); break;
    case 'S': init_S(); break;
    case 'L': init_L(); break;
    case 'E': init_E(); break;
    case 'N': init_N(); break;
    case 'D': init_D(); break;
    case ':': init_semicolon(); break;
    case '.': init_point(); break;
    case ',': init_comma(); break;
    case '-': init_minus(); break;
    case 'l': init_l(); break;
    case 'v': init_v(); break;
    case 'c': init_c(); break;
    case 'a': init_a(); break;
    case 'r': init_r(); break;
    case 'e': init_e(); break;
    case 'd': init_d(); break;
    case '!': init_exclaim(); break;

    }
    initGL();
    FlyingObject::init();
}
void Letter::initGL()
{
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(Point), vertices, GL_STATIC_DRAW);

}
void Letter::init_0()
{
    mkPoints(9);
    vertices[0] = Point(-0.3, 1.0);
    vertices[1] = Point(0.3, 1.0);
    vertices[2] = Point(0.5, 0.8);
    vertices[3] = Point(0.5, -0.8);
    vertices[4] = Point(0.3, -1.0);
    vertices[5] = Point(-0.3, -1.0);
    vertices[6] = Point(-0.5, -0.8);
    vertices[7] = Point(-0.5, 0.8);
    vertices[8] = Point(-0.3, 1.0);

}

void Letter::init_1()
{
    mkPoints(3);
    vertices[0] = Point(-0.15, 0.5);
    vertices[1] = Point( 0.15, 1.0);
    vertices[2] = Point(0.15, -1.0);
    width = 1.3;
}

void Letter::init_2()
{
    mkPoints(6);
    vertices[0] = Point(-0.5, 0.8);
    vertices[1] = Point(-0.3, 1.0);
    vertices[2] = Point(0.3, 1.0);
    vertices[3] = Point(0.5, 0.8);
    vertices[4] = Point(-0.5, -1.0);
    vertices[5] = Point(0.5, -1.0);

}

void Letter::init_3()
{
    mkPoints(11);
    vertices[0] = Point(-0.5, 0.8);
    vertices[1] = Point(-0.3, 1.0);
    vertices[2] = Point(0.3, 1.0);
    vertices[3] = Point(0.5, 0.8);
    vertices[4] = Point(0.5, 0.2);
    vertices[5] = Point(0.1, 0.0);
    vertices[6] = Point(0.5, -0.2);
    vertices[7] = Point(0.5, -0.8);
    vertices[8] = Point(0.3, -1.0);
    vertices[9] = Point(-0.3, -1.0);
    vertices[10] = Point(-0.5, -0.8);

}

void Letter::init_4()
{
    mkPoints(5);
    vertices[0] = Point(-0.5, 1.0);
    vertices[1] = Point(-0.5, 0.0);
    vertices[2] = Point(0.5, 0.0);
    vertices[3] = Point(0.5, 1.0);
    vertices[4] = Point(0.5,-1.0);

}

void Letter::init_5()
{
    mkPoints(9);
    vertices[0] = Point(0.5, 1.0);
    vertices[1] = Point(-0.5, 1.0);
    vertices[2] = Point(-0.5, 0.0);
    vertices[3] = Point(0.3, 0.0);
    vertices[4] = Point(0.5, -0.2);
    vertices[5] = Point(0.5, -0.8);
    vertices[6] = Point(0.3, -1.0);
    vertices[7] = Point(-0.3, -1.0);
    vertices[8] = Point(-0.5, -0.8);
}
void Letter::init_6()
{
    mkPoints(11);
    vertices[0] = Point(0.5, 0.8);
    vertices[1] = Point(0.3, 1.0);
    vertices[2] = Point(-0.3, 1.0);
    vertices[3] = Point(-0.5, 0.8);
    vertices[4] = Point(-0.5, -0.8);
    vertices[5] = Point(-0.3, -1.0);
    vertices[6] = Point(0.3, -1.0);
    vertices[7] = Point(0.5, -0.8);
    vertices[8] = Point(0.5, -0.2);
    vertices[9] = Point(0.3, 0.0);
    vertices[10] = Point(-0.5, -0.0);

}

void Letter::init_7()
{
    mkPoints(3);
    vertices[0] = Point(-0.5, 1.0);
    vertices[1] = Point(0.5, 1.0);
    vertices[2] = Point(-0.1, -1.0);

}

void Letter::init_8()
{
    mkPoints(17);
    vertices[0] = Point(-0.3, 0.0);
    vertices[1] = Point(-0.5, 0.2);
    vertices[2] = Point(-0.5, 0.8);
    vertices[3] = Point(-0.3, 1.0);
    vertices[4] = Point(0.3, 1.0);
    vertices[5] = Point(0.5, 0.8);
    vertices[6] = Point(0.5, 0.2);
    vertices[7] = Point(0.3, 0.0);
    vertices[8] = Point(-0.3, -0.0);
    vertices[9] = Point(-0.5, -0.2);
    vertices[10] = Point(-0.5, -0.8);
    vertices[11] = Point(-0.3, -1.0);
    vertices[12] = Point(0.3, -1.0);
    vertices[13] = Point(0.5, -0.8);
    vertices[14] = Point(0.5, -0.2);
    vertices[15] = Point(0.3, 0.0);
    vertices[16] = Point(-0.3, 0.0);

}

void Letter::init_9()
{
    mkPoints(12);
    vertices[0] = Point(0.5, 0.2);
    vertices[1] = Point(0.3, 0.0);
    vertices[2] = Point(-0.3, 0.0);
    vertices[3] = Point(-0.5, 0.2);
    vertices[4] = Point(-0.5, 0.8);
    vertices[5] = Point(-0.3, 1.0);
    vertices[6] = Point(0.3, 1.0);
    vertices[7] = Point(0.5, 0.8);
    vertices[8] = Point(0.5, -0.8);
    vertices[9] = Point(0.3, -1.0);
    vertices[10] = Point(-0.3, -1.0);
    vertices[11] = Point(-0.5, -0.8);
}

void Letter::init_S()
{
    mkPoints(12);
    vertices[0] = Point(0.5, 0.8);
    vertices[1] = Point(0.3, 1.0);
    vertices[2] = Point(-0.3, 1.0);
    vertices[3] = Point(-0.5, 0.8);
    vertices[4] = Point(-0.5, 0.2);
    vertices[5] = Point(-0.3, 0.0);
    vertices[6] = Point(0.3, 0.0);
    vertices[7] = Point(0.5, -0.2);
    vertices[8] = Point(0.5, -0.8);
    vertices[9] = Point(0.3, -1.0);
    vertices[10] = Point(-0.3, -1.0);
    vertices[11] = Point(-0.5, -0.8);

}

void Letter::init_L()
{
    mkPoints(3);
    vertices[0] = Point(-0.5, 1.0);
    vertices[1] = Point(-0.5, -1.0);
    vertices[2] = Point(0.5, -1.0);
}

void Letter::init_E()
{
    mkPoints(7);
    vertices[0] = Point(0.5, 1.0);
    vertices[1] = Point(-0.5, 1.0);
    vertices[2] = Point(-0.5, 0.0);
    vertices[3] = Point(0.5, 0.0);
    vertices[4] = Point(-0.5, 0.0);
    vertices[5] = Point(-0.5, -1.0);
    vertices[6] = Point(0.5, -1.0);

}

void Letter::init_N()
{
    mkPoints(4);
    vertices[0] = Point(0.5, 1.0);
    vertices[1] = Point(0.5, -1.0);
    vertices[2] = Point(-0.5, 1.0);
    vertices[3] = Point(-0.5, -1.0);

}

void Letter::init_D()
{
    mkPoints(7);
    vertices[0] = Point(-0.5, 1.0);
    vertices[1] = Point(-0.5, -1.0);
    vertices[2] = Point(0.3, -1.0);
    vertices[3] = Point(0.5, -0.8);
    vertices[4] = Point(0.5, 0.8);
    vertices[5] = Point(0.3, 1.0);
    vertices[6] = Point(-0.5, 1.0);

}
void Letter::init_l()
{
    what = GL_LINES;
    mkPoints(2);
    vertices[0] = Point(0.0, -1.0);
    vertices[1] = Point(0.0, 1.0);

}

void Letter::init_v()
{
    mkPoints(3);
    vertices[0] = Point(-0.5, 0.2);
    vertices[1] = Point(0.0, -1.0);
    vertices[2] = Point(0.5, 0.2);

}

void Letter::init_c()
{
    mkPoints(8);
    vertices[0] = Point(0.5, 0.0);
    vertices[1] = Point(0.3, 0.2);
    vertices[2] = Point(-0.3, 0.2);
    vertices[3] = Point(-0.5, 0.0);
    vertices[4] = Point(-0.5, -0.8);
    vertices[5] = Point(-0.3, -1.0);
    vertices[6] = Point(0.3, -1.0);
    vertices[7] = Point(0.5, -0.8);
}

void Letter::init_a()
{
    mkPoints(14);
    int i=0;
    vertices[i++] = Point(0.5, -1.0);
    vertices[i++] = Point(0.3, -0.8);
    vertices[i++] = Point(0.3, -0.1);
    vertices[i++] = Point(-0.3, -0.1);
    vertices[i++] = Point(-0.5, -0.3);
    vertices[i++] = Point(-0.5, -0.8);
    vertices[i++] = Point(-0.5, -0.8);
    vertices[i++] = Point(-0.3, -1.0);
    vertices[i++] = Point(0.1, -1.0);
    vertices[i++] = Point(0.3, -0.8);
    vertices[i++] = Point(0.3, 0.1);
    vertices[i++] = Point(0.1, 0.3);
    vertices[i++] = Point(-0.3, 0.3);
    vertices[i++] = Point(-0.5, 0.3);

}

void Letter::init_r()
{
    mkPoints(6);
    int i=0;
    vertices[i++] = Point(-0.5, 0.2);
    vertices[i++] = Point(-0.5, -1.0);
    vertices[i++] = Point(-0.5, 0.0);
    vertices[i++] = Point(-0.3, 0.2);
    vertices[i++] = Point(-0.3, 0.2);
    vertices[i++] = Point( 0.5, 0.0);

}

void Letter::init_e()
{
    mkPoints(10);
    int i=0;
    vertices[i++] = Point(0.5, -0.8);
    vertices[i++] = Point(0.3, -1.0);
    vertices[i++] = Point(-0.3, -1.0);
    vertices[i++] = Point(-0.5, -0.8);
    vertices[i++] = Point(-0.5, 0.0);
    vertices[i++] = Point(-0.3, 0.2);
    vertices[i++] = Point(0.3, 0.2);
    vertices[i++] = Point( 0.5, 0.0);
    vertices[i++] = Point( 0.5, -0.2);
    vertices[i++] = Point(-0.5, -0.2);

}

void Letter::init_d()
{
    mkPoints(10);
    int i=0;
    vertices[i++] = Point(0.5, -1.0);
    vertices[i++] = Point(0.5, 0.0);
    vertices[i++] = Point(0.3, 0.2);
    vertices[i++] = Point(-0.3, 0.2);
    vertices[i++] = Point(-0.5, 0.0);
    vertices[i++] = Point(-0.5, -0.8);
    vertices[i++] = Point(-0.3, -1.0);
    vertices[i++] = Point(0.3, -1.0);
    vertices[i++] = Point(0.5, -0.8);
    vertices[i++] = Point(0.5, 1.0);

}

void Letter::init_exclaim()
{
    what = GL_LINES;
    mkPoints(4);
    vertices[0] = Point(0.0, 1.0);
    vertices[1] = Point(0.0, -0.1);
    vertices[2] = Point(0.0, -0.8);
    vertices[3] = Point(0.0, -1.0);
}


void Letter::init_semicolon()
{
    what = GL_LINES;
    linewidth = 2.0;
    mkPoints(4);
    vertices[0] = Point(0.0, -0.6);
    vertices[1] = Point(0.0, -0.2);
    vertices[2] = Point(0.0, 0.2);
    vertices[3] = Point(0.0, 0.6);

}

void Letter::init_point()
{
    what = GL_LINES;
    linewidth = 2.0;
    mkPoints(2);
    vertices[0] = Point(0.0, -0.6);
    vertices[1] = Point(0.0, -0.2);
}

void Letter::init_comma()
{
    what = GL_LINES;
    linewidth = 2.0;
    mkPoints(2);
    vertices[0] = Point(0.0, -0.6);
    vertices[1] = Point(-0.2, -0.2);
}

void Letter::init_minus()
{
    what = GL_LINES;
    linewidth = 2.0;
    mkPoints(2);
    vertices[0] = Point(-0.4, -0.0);
    vertices[1] = Point(0.4, -0.0);
}


Text::Text(View* view)
{
    letters['0'] = new Letter(view, '0');
    letters['1'] = new Letter(view, '1');
    letters['2'] = new Letter(view, '2');
    letters['3'] = new Letter(view, '3');
    letters['4'] = new Letter(view, '4');
    letters['5'] = new Letter(view, '5');
    letters['6'] = new Letter(view, '6');
    letters['7'] = new Letter(view, '7');
    letters['8'] = new Letter(view, '8');
    letters['9'] = new Letter(view, '9');
    letters['S'] = new Letter(view, 'S');
    letters['L'] = new Letter(view, 'L');
    letters['E'] = new Letter(view, 'E');
    letters['N'] = new Letter(view, 'N');
    letters['D'] = new Letter(view, 'D');
    letters[':'] = new Letter(view, ':');
    letters['.'] = new Letter(view, '.');
    letters[','] = new Letter(view, ',');
    letters['-'] = new Letter(view, '-');
    letters['c'] = new Letter(view, 'c');
    letters['l'] = new Letter(view, 'l');
    letters['e'] = new Letter(view, 'e');
    letters['a'] = new Letter(view, 'a');
    letters['r'] = new Letter(view, 'r');
    letters['d'] = new Letter(view, 'd');
    letters['v'] = new Letter(view, 'v');
    letters['!'] = new Letter(view, '!');
}

Text::~Text()
{
}

void Text::draw(float x, float y, float scale, Point4D color,float lwidth,  const char *txt)
{
    float left = x;
    for (unsigned int i=0; i< strlen(txt); i++)
    {
        char c = txt[i];
        Letter *l = letters[c];
        if (!l)
        {
            left = left + 1.0 * scale;
            continue;
        }
        l->setScale (scale);
        l->setX(left - (2-l->width)*scale* 0.5);
        l->setY(y);
        l->setColor(color);
        l->linewidth = lwidth,
        l->draw();
        left = left + l->width * scale;
    }
}

void Text::drawCenter(float x, float y, float scale, Point4D color, float lwidth, const char *txt)
{
    float w = textWidth(txt)* scale;
    draw(x- w /2, y, scale, color, lwidth, txt);
}
void Text::initGL()
{
	std::map<char, Letter* >::iterator it = letters.begin();
	for (; it != letters.end(); it++)
        (*it).second->initGL();
}

float Text::textWidth(const char *txt)
{
    float w = 0.0;
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
