#include "JniThreads.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

struct NativeWorkerArgs
{
    jint id;
    jint iterations;
};

static jmethodID gOnNativeMessage = NULL;
static JavaVM * gVm = NULL;
static jobject gObj = NULL;
static pthread_mutex_t mutex;

static void* natvieWorkerThread(void *args);

jint JNI_OnLoad
(
    JavaVM * vm,
    void * reserved
)
{
    gVm = vm;
    return JNI_VERSION_1_4;
}


/*
 * Class:     com_airensoft_jnithreads_app_NDKModule
 * Method:    nativeInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_airensoft_jnithreads_app_MainActivity_nativeInit
(
    JNIEnv * env,
    jobject  obj
)
{
    if(0 != pthread_mutex_init(&mutex, NULL)) {
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exceptionClazz, "Unable to initialize mutex");
        goto exit;
    }


    if(NULL == gObj) {
        gObj = env->NewGlobalRef(obj);
        if(NULL == gObj) {
            goto exit;
        }
    }

    if(NULL == gOnNativeMessage) {
        jclass clazz = env->GetObjectClass(obj);

        gOnNativeMessage = env->GetMethodID(clazz, "onNativeMessage", "(Ljava/lang/String;)V");

        if(NULL == gOnNativeMessage) {
            jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
            env->ThrowNew(exceptionClazz, "Unable to find method");
        }
    }

exit:
    return;
}

/*
 * Class:     com_airensoft_jnithreads_app_NDKModule
 * Method:    nativeFree
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_airensoft_jnithreads_app_MainActivity_nativeFree
(
    JNIEnv * env,
    jobject  obj
)
{
    if(NULL != gObj) {
        env->DeleteGlobalRef(gObj);
        gObj = NULL;
    }

    if(0 != pthread_mutex_destroy(&mutex)) {
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exceptionClazz, "Unable to destroy mutex");
    }
}

/*
 * Class:     com_airensoft_jnithreads_app_NDKModule
 * Method:    nativeWorker
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_airensoft_jnithreads_app_MainActivity_nativeWorker
(
    JNIEnv * env,
    jobject  obj,
    jint     id,
    jint     iterations
)
{
    if(0 != pthread_mutex_lock(&mutex)) {
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exceptionClazz, "Unable to lock mutex");
        goto exit;
    }

    for(jint i=0; i<iterations; i++) {
        char message[26];
        sprintf(message, "Worker %d: Iteration %d", id, i);

        jstring messageString = env->NewStringUTF(message);

        env->CallVoidMethod(obj, gOnNativeMessage, messageString);

        if(NULL != env->ExceptionOccurred())
            break;

        sleep(1);
    }

    if(0 != pthread_mutex_unlock(&mutex)) {
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exceptionClazz, "Unable to unlock mutex");
        goto exit;
    }
exit:
    return;
}


JNIEXPORT void JNICALL Java_com_airensoft_jnithreads_app_MainActivity_posixThreads
(
    JNIEnv * env,
    jobject  obj,
    jint     threads,
    jint     iterations
)
{
    pthread_t * handles = new pthread_t[threads];

    for(jint i = 0; i<threads; i++) {
        NativeWorkerArgs * nativeWorkerArgs = new NativeWorkerArgs();
        nativeWorkerArgs->id = i;
        nativeWorkerArgs->iterations = iterations;

        int result = pthread_create(&handles[i], NULL, natvieWorkerThread, (void *)nativeWorkerArgs);

        if(0 != result) {
            jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
            env->ThrowNew(exceptionClazz, "Unable to create thread");
            goto exit;
        }
    }

//    for(jint i=0; i<threads; i++) {
//        void * result = NULL;
//
//        if(0 != pthread_join(handles[i], &result)) {
//            jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
//            env->ThrowNew(exceptionClazz, "Unable to join thread");
//        } else {
//            char message[26];
//            sprintf(message, "Worker %d returned %d", i, *((int *)result));
//
//            jstring messageString = env->NewStringUTF(message);
//            env->CallVoidMethod(obj, gOnNativeMessage, messageString);
//
//            if(NULL != env->ExceptionOccurred()) {
//                goto exit;
//            }
//        }
//    }
exit:
    return;
}


static void* natvieWorkerThread(void *args)
{
    JNIEnv * env = NULL;

    if(0 == gVm->AttachCurrentThread(&env, NULL)) {

        NativeWorkerArgs * nativeWorkerArgs = (NativeWorkerArgs *)args;

        Java_com_airensoft_jnithreads_app_MainActivity_nativeWorker(env, gObj, nativeWorkerArgs->id, nativeWorkerArgs->iterations);
        delete nativeWorkerArgs;

        gVm->DetachCurrentThread();
    }
    return (void *)1;
}