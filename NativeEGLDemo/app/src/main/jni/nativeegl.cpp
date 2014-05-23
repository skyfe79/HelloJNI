#include <stdint.h>
#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "logger.h"
#include "renderer.h"
#include "nativeegl.h"

#define LOG_TAG "EglSample"



static ANativeWindow *window = 0;
static Renderer *renderer = 0;

/*
 * Class:     com_airensoft_nativeegldemo_app_NativeEGL
 * Method:    nativeOnStart
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_airensoft_nativeegldemo_app_NativeEGL_nativeOnStart
(
    JNIEnv * jenv,
    jclass   jclz
)
{
    LOG_INFO("nativeOnStart");
    renderer = new Renderer();
}

/*
 * Class:     com_airensoft_nativeegldemo_app_NativeEGL
 * Method:    nativeOnResume
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_airensoft_nativeegldemo_app_NativeEGL_nativeOnResume
(
    JNIEnv * jenv,
    jclass   jclz
)
{
    LOG_INFO("nativeOnResume");
    renderer->start();
}


/*
 * Class:     com_airensoft_nativeegldemo_app_NativeEGL
 * Method:    nativeOnPause
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_airensoft_nativeegldemo_app_NativeEGL_nativeOnPause
(
    JNIEnv * jenv,
    jclass   jclz
)
{
    LOG_INFO("nativeOnPause");
    renderer->stop();
}


/*
 * Class:     com_airensoft_nativeegldemo_app_NativeEGL
 * Method:    nativeOnStop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_airensoft_nativeegldemo_app_NativeEGL_nativeOnStop
(
    JNIEnv * jenv,
    jclass   jclz
)
{
    LOG_INFO("nativeOnStop");
    delete renderer;
    renderer = 0;
}


/*
 * Class:     com_airensoft_nativeegldemo_app_NativeEGL
 * Method:    nativeSetSurface
 * Signature: (Landroid/view/Surface;)V
 */
JNIEXPORT void JNICALL Java_com_airensoft_nativeegldemo_app_NativeEGL_nativeSetSurface
(
    JNIEnv * jenv,
    jclass   jclz,
    jobject  jsurface
)
{
    if(jsurface != 0)
    {
        window = ANativeWindow_fromSurface(jenv, jsurface);
        LOG_INFO("Got window %p", window);
        renderer->setWindow(window);
    }
    else
    {
        LOG_INFO("Releasing window");
        ANativeWindow_release(window);
    }
}

