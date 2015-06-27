#include <jni.h>
#include "game.h"
#include "glwrapper.h"
#include <android/log.h>
#include "../game/view.h"
#include <time.h>

View* view = 0;
void on_surface_created(JNIEnv* _env, jclass cls)
{

	view->initializeGL();
	view->setPause(false);
}

void on_surface_changed(int width, int height)
{
	view->resizeGL(width, height);
}

int on_draw_frame() {
	if (view->getPause())
		return true;
	glClear(GL_COLOR_BUFFER_BIT);
	long long msec = 1.0* clock() / CLOCKS_PER_SEC * 1000;
	return view->drawFrame(msec);
}
void on_touch_event(jint what, jint x, jint y)
{
	view->onTouchEvent(what, x, y);
}
void set_pause(bool p)
{
	view->setPause(p);
}
int scores()
{
	if (view)
		return view->scores();
	return 0;
}

void new_game()
{
	if (!view)
		view = new View ();
	else
		view->newGame();
}
