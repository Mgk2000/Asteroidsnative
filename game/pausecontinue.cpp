#include "pausecontinue.h"
#include "view.h"
#include "logmsg.h"

PauseContinue::PauseContinue(View* view, Texture* _pauseTexture, Texture* _continueTexture) : FlyingObject(view,1,0),
  pauseTexture(_pauseTexture), continueTexture(_continueTexture), lastPressTime(-999999999999)
{
    setPause(false);
}

void PauseContinue::init()
{

    nvertices = 4;
    _r = 1;
    vertices = new Point[nvertices];
    vertices[0] = Point(-_r, _r);
    vertices[1] = Point(-_r, -_r);
    vertices[2] = Point(_r, -_r);
    vertices[3] = Point(_r, _r);
    initGL();
}
void PauseContinue::initGL()
{
    Point4D* vertices4 = new Point4D[6];
    vertices4[0] = Point4D (0 , 0, 0.5 , 0.5);
    vertices4[1] = Point4D(vertices[0].x, vertices[0].y, 0,1);
    vertices4[2] = Point4D(vertices[1].x, vertices[1].y, 0,0);
    vertices4[3] = Point4D(vertices[2].x, vertices[2].y, 1,0);
    vertices4[4] = Point4D(vertices[3].x, vertices[3].y, 1,1);
    vertices4[5] = vertices4[1];
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, (nvertices+2) * sizeof(Point4D), vertices4, GL_STATIC_DRAW);
    delete[] vertices4;

}

void PauseContinue::setPause(bool _pause)
{
    if (_pause)
    {
        x = 0.0;
        y = 0.5;
        _scale = 0.1;
        _texture = continueTexture;
    }
    else
    {
        x = 0.5;
        y = 0.95;
        _scale = 0.05;
        _texture = pauseTexture;
    }
    bLeft = x - _scale*2;
    bTop = y + _scale*2;
    bRight = x + _scale*2;
    bBottom = y - _scale*2;
//    view->setPause(_pause);
}
void PauseContinue::processTouchPause(float fx, float fy)
{
	if (currTime()-lastPressTime<200)
		return;
	lastPressTime = currTime();
    if (fx >= bLeft && fx <= bRight && fy <= bTop && fy >= bBottom)
    {
    	LOGD("Touch pause fx=%f fy=%f", fx, fy);
        bool p = !view->getPause();
        //if (!p)
        //	return;
        setPause(p);
        view->setPPause(p);
    }
}

void PauseContinue::draw()
{
    drawTexture();
}
