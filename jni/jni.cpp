#include "game.h"
#include <jni.h>
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_game_asteroidsnative_GameLibJNIWrapper_on_1surface_1created
(JNIEnv * _env, jclass cls)
{
	on_surface_created(_env, cls);
}

JNIEXPORT void JNICALL Java_com_game_asteroidsnative_GameLibJNIWrapper_on_1surface_1changed
(JNIEnv * _env, jclass cls, jint width, jint height) {
	on_surface_changed((int) width, (int) height);
}

JNIEXPORT int JNICALL Java_com_game_asteroidsnative_GameLibJNIWrapper_on_1draw_1frame
(JNIEnv * _env, jclass cls)
{
	int i = on_draw_frame();
	return i;
}

JNIEXPORT void JNICALL Java_com_game_asteroidsnative_GameLibJNIWrapper_on_1touch_1event(JNIEnv * _env, jclass cls,
		jint what, jint x, jint y)
{
	on_touch_event(what, x, y);
}

JNIEXPORT void JNICALL Java_com_game_asteroidsnative_GameLibJNIWrapper_set_1pause
  (JNIEnv * _env, jclass cls, jboolean p)
{
	set_pause((bool)p);
}

JNIEXPORT void JNICALL Java_com_game_asteroidsnative_GameLibJNIWrapper_new_1game
  (JNIEnv * _env, jclass cls)
{
	new_game();
}


JNIEXPORT jint JNICALL Java_com_game_asteroidsnative_GameLibJNIWrapper_scores
  (JNIEnv * _env, jclass cls)
{
	return scores();
}
#ifdef __cplusplus
};
#endif


