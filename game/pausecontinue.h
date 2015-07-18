#ifndef PAUSECONTINUE_H
#define PAUSECONTINUE_H
#include "flyingobject.h"
class Texture;
class PauseContinue : public FlyingObject
{
public:
    PauseContinue(View* view, Texture* _pauseTexture, Texture* _continueTexture);
    void init();
    void initGL();
    void setPause(bool _pause);
    void processTouchPause(float fx, float fy);
    void draw();
private:
    Texture* pauseTexture, *continueTexture;
    float bLeft, bRight, bTop, bBottom;
    long long lastPressTime;
};

#endif // PAUSECONTINUE_H
