#include "mylib.h"

JNIEXPORT jstring JNICALL Java_com_airensoft_jni01_app_HelloJNI_getHelloString(JNIEnv * env, jclass clazz)
{
    return (*env)->NewStringUTF(env, "Hello JNI GOGOGO!");
}