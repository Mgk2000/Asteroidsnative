LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := Asteroidsnative
LOCAL_SRC_FILES := game.cpp jni.cpp ../game/view.cpp
LOCAL_SRC_FILES += ../game/random.cpp
LOCAL_SRC_FILES += ../game/bullet.cpp
LOCAL_SRC_FILES += ../game/patrolbullet.cpp
LOCAL_SRC_FILES += ../game/flyingobject.cpp
LOCAL_SRC_FILES += ../game/ship.cpp
LOCAL_SRC_FILES += ../game/gun.cpp
LOCAL_SRC_FILES += ../game/asteroid.cpp
LOCAL_SRC_FILES += ../game/patrol.cpp
LOCAL_SRC_FILES += ../game/mat4.cpp
LOCAL_SRC_FILES += ../game/math_helper.cpp
LOCAL_SRC_FILES += ../game/text.cpp
LOCAL_SRC_FILES += ../game/mutex.cpp
LOCAL_LDLIBS := -lGLESv2 -llog
LOCAL_C_INCLUDES += ${NDKROOT}/sources/cxx-stl/stlport/stlport
LOCAL_C_INCLUDES += ../game

include $(BUILD_SHARED_LIBRARY)
ifndef NDK_ROOT
include external/stlport/libstlport.mk
endif
