#ifndef GAME_H
#define GAME_H

#include <android/log.h>
#include <jni.h>
void on_surface_created(JNIEnv* _env, jclass _cls);
void on_surface_changed(int width, int height);
int on_draw_frame();
void on_touch_event(int what, int x, int y);
void set_pause(bool p);
void new_game();
int scores();

#define  LOG_TAG    "Aster"

#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

#endif