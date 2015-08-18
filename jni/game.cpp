#include <jni.h>
#include "game.h"
#include "glwrapper.h"
#include <android/log.h>
#include "../game/view.h"
#include <time.h>

View* view = 0;
void on_surface_created(JNIEnv* _env, jclass cls, jboolean russian)
{

//	LOGD("on_surface_created 1 view=%d", (int) view);
	view->initializeGL(russian);
//	LOGD("on_surface_created 2");
	//view->setPause(false);
//	LOGD("on_surface_created 3");
}

void on_surface_changed(int width, int height)
{
	view->resizeGL(width, height);
}

int on_draw_frame() {
//	if (view->getPause())
//		return true;
	return view->drawFrame();
}
void on_touch_event(jint what, jint x, jint y)
{
	view->onTouchEvent(what, x, y);
}
void set_pause(bool p)
{
	//view->setPause(p);
}
int scores()
{
	if (view)
		return view->scores();
	return 0;
}

void new_game()
{
	//LOGD("new_game view=%d", (int) view);
//	if (!view)
//		view = new View ();
//	else
		view->newGame();
}

void create_game()
{
	if (!view)
		view = new View();
}

void add_texture (int w, int h, const char* data, int kind, bool whiteTransparent)
{
	view->addTexture(w,h, data, kind, whiteTransparent);
}
