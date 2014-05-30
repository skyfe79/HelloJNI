extern "C" {
#include <avilib.h>
}

#include <android/bitmap.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <malloc.h>

#include "common.h"
#include "AVIPlayer.h"


struct Instance
{
    char *  buffer;
    GLuint  texture;
    Instance() : buffer(0), texture(0)
    {}
};

/*
 * Class:     com_airensoft_aviplayer_app_AVIPlayer
 * Method:    open
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_airensoft_aviplayer_app_AVIPlayer_open
(
    JNIEnv * env,
    jclass clazz,
    jstring fileName
)
{
    avi_t * avi = 0;

    const char * cFileName = env->GetStringUTFChars(fileName, 0);
    if(0 == cFileName)
    {
        goto exit;
    }

    avi = AVI_open_input_file(cFileName, 1);
    env->ReleaseStringUTFChars(fileName, cFileName);

    if(0 == avi)
    {
        ThrowException(env, "java/io/IOException", AVI_strerror());
    }

exit:
    return (jlong)avi;
}

/*
 * Class:     com_airensoft_aviplayer_app_AVIPlayer
 * Method:    getWidth
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_airensoft_aviplayer_app_AVIPlayer_getWidth
(
    JNIEnv * env,
    jclass clazz,
    jlong  avi
)
{
    return AVI_video_width((avi_t*)avi);
}



/*
 * Class:     com_airensoft_aviplayer_app_AVIPlayer
 * Method:    getHeight
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_airensoft_aviplayer_app_AVIPlayer_getHeight
(
    JNIEnv * env,
    jclass clazz,
    jlong  avi
)
{
    return AVI_video_height((avi_t*)avi);
}

/*
 * Class:     com_airensoft_aviplayer_app_AVIPlayer
 * Method:    getFrameRate
 * Signature: (J)D
 */
JNIEXPORT jdouble JNICALL Java_com_airensoft_aviplayer_app_AVIPlayer_getFrameRate
(
    JNIEnv * env,
    jclass clazz,
    jlong  avi
)
{
    return AVI_frame_rate((avi_t*)avi);
}


/*
 * Class:     com_airensoft_aviplayer_app_AVIPlayer
 * Method:    close
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_airensoft_aviplayer_app_AVIPlayer_close
(
    JNIEnv * env,
    jclass   clazz,
    jlong    avi
)
{
    AVI_close((avi_t *)avi);
}


JNIEXPORT jboolean JNICALL Java_com_airensoft_aviplayer_app_AVIPlayer_render
(
    JNIEnv * env,
    jclass   clazz,
    jlong    avi,
    jobject  bitmap
)
{
    jboolean isFrameRead = JNI_FALSE;

    char * frameBuffer = 0;
    long frameSize = 0;
    int  keyFrame = 0;

    if(0 > AndroidBitmap_lockPixels(env, bitmap, (void **)&frameBuffer))
    {
        ThrowException(env, "java/io/IOException", "Unable to lock pixels");
        goto exit;
    }

    frameSize = AVI_read_frame((avi_t *)avi, frameBuffer, &keyFrame);

    if(0 > AndroidBitmap_unlockPixels(env, bitmap))
    {
        ThrowException(env, "java/io/IOException", "Unable to unlock pixels");
        goto exit;
    }

    if(0 < frameSize)
    {
        isFrameRead = JNI_TRUE;
    }
exit:
    return isFrameRead;
}




JNIEXPORT jboolean JNICALL Java_com_airensoft_aviplayer_app_AVIPlayer_glrender
(
    JNIEnv *    env,
    jclass      clazz,
    jlong       inst,
    jlong       avi
)
{
    Instance * instance = (Instance *)inst;
    jboolean isFrameRead = JNI_FALSE;
    int keyFrame = 0;

    long frameSize = AVI_read_frame((avi_t*)avi, instance->buffer, &keyFrame);
    if(0 >= frameSize)
    {
        goto exit;
    }

    isFrameRead = JNI_FALSE;

    glTexSubImage2D(GL_TEXTURE_2D,
        0,
        0,
        0,
        AVI_video_width((avi_t*)avi),
        AVI_video_height((avi_t*)avi),
        GL_RGB,
        GL_UNSIGNED_SHORT_5_6_5,
        instance->buffer);

    glDrawTexiOES(
        0,
        0,
        0,
        AVI_video_width((avi_t*)avi),
        AVI_video_height((avi_t*)avi));

exit:
    return isFrameRead;
}

JNIEXPORT jlong JNICALL Java_com_airensoft_aviplayer_app_AVIPlayer_init
(
    JNIEnv *    env,
    jclass      clazz,
    jlong       avi
)
{
    Instance * instance = 0;
    long frameSize = AVI_frame_size((avi_t*)avi, 0);
    if(0 >= frameSize)
    {
        ThrowException(env, "java/io/RuntimeException", "Unable to get the frame size");
        goto exit;
    }

    instance = new Instance();
    if(0 == instance)
    {
        ThrowException(env, "java/io/RuntimeException", "Unable to allocate instance");
        goto exit;
    }

    instance->buffer = (char *)malloc(frameSize);
    if(0 == instance->buffer)
    {
        ThrowException(env, "java/io/RuntimeException", "Unable to allocate buffer");
        delete instance;
        goto exit;
    }
exit:
    return (jlong)instance;
}


JNIEXPORT void JNICALL Java_com_airensoft_aviplayer_app_AVIPlayer_initSurface
(
    JNIEnv *    env,
    jclass      clazz,
    jlong       inst,
    jlong       avi
)
{
    Instance * instance = (Instance *)inst;

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &instance->texture);
    glBindTexture(GL_TEXTURE_2D, instance->texture);

    int frameWidth = AVI_video_width((avi_t*)avi);
    int frameHeight= AVI_video_height((avi_t*)avi);

    GLint rect[] = {0, frameHeight, frameWidth, -frameHeight};
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, rect);

    glColor4f(1.0, 1.0, 1.0, 1.0);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGB,
        frameWidth,
        frameHeight,
        0,
        GL_RGB,
        GL_UNSIGNED_SHORT_5_6_5,
        0);
}

JNIEXPORT void JNICALL Java_com_airensoft_aviplayer_app_AVIPlayer_free
(
    JNIEnv *    env,
    jclass      clazz,
    jlong       inst
)
{
    Instance * instance = (Instance *)inst;

    if(0 != instance)
    {
        free(instance->buffer);
        delete instance;
    }
}

