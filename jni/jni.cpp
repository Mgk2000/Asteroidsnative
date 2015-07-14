#include "game.h"
#include <jni.h>
#include <cstdio>
#include <cstring>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
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

JNIEXPORT void JNICALL Java_com_game_asteroidsnative_GameLibJNIWrapper_create_1game
  (JNIEnv * _env, jclass cls)
{
	create_game();
}


JNIEXPORT jint JNICALL Java_com_game_asteroidsnative_GameLibJNIWrapper_scores
  (JNIEnv * _env, jclass cls)
{
	return scores();
}

JNIEXPORT jint JNICALL Java_com_game_asteroidsnative_GameLibJNIWrapper_add_1texture
  (JNIEnv * env, jclass cls, jobject srcassetManager, jstring assetName, int kind)
{
    AAssetManager* assetManager = AAssetManager_fromJava(env, srcassetManager);
    const char *szAssetName = env->GetStringUTFChars(assetName, JNI_FALSE);
    AAsset* asset = AAssetManager_open(assetManager, szAssetName, AASSET_MODE_RANDOM);
    env->ReleaseStringUTFChars(assetName, szAssetName);
//    	LOGD("Exists");
   	int len = AAsset_getLength(asset);
   	char* buf = new char[len];
   	int nb = AAsset_read (asset, buf, len);
   	int szbuf[2];
   	memcpy(szbuf, &buf[18],8);

  //  	LOGD("After read len=%d", len);
    //	LOGD("buf=%s", buf);
    AAsset_close(asset);
	add_texture(szbuf[0], szbuf[1], &buf[54], kind, true);
}
JNIEXPORT void JNICALL Java_com_game_asteroidsnative_GameLibJNIWrapper_add_1intarr_1texture
  (JNIEnv * env, jclass cls, jint w, jint h, jintArray dataArr, jint kind, jboolean whiteTransparent)
{
	LOGD("add_1intarr_1texture 1");
	int* buf = new int[ w * h];
	char* data = new char[ w * h * 3];
	LOGD("add_1intarr_1texture 2");
	jint length = env->GetArrayLength(dataArr);
	LOGD("add_1intarr_1texture 3 length=%d", length);

    env->GetIntArrayRegion(dataArr, 0, length, (jint*)buf);
	LOGD("w=%d h=%d length=%d buf=%x", w,h,length, buf[0]);
	for (int i=0; i< h; i++)
		for (int j =0; j< w; j++)
		{
			int ii = h-i-1;
			memcpy(&data[i*3 * w + j*3 ], &buf[ii *w + j], 3);
		}
	add_texture(w, h, data, kind, whiteTransparent);
	LOGD("Png texture added");
}
#ifdef __cplusplus
};
#endif


