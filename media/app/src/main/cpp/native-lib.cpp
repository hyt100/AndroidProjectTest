#include <jni.h>
#include <stdint.h>
#include <string>
#include <pthread.h>
#include <android/log.h>
#include "jni/com_example_media_MediaSt.h"
#include "MediaLiveStreamListener.h"

#define LOG_ERROR(format, ...)     __android_log_print(ANDROID_LOG_ERROR, "medialib", format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)     __android_log_print(ANDROID_LOG_INFO, "medialib", format, ##__VA_ARGS__)

static JavaVM *g_JavaVM = NULL;


jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv *env = NULL;
    jint result = JNI_ERR;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    g_JavaVM = vm;

    return JNI_VERSION_1_4;
}

JNIEnv *getJNIEnv()
{
    JNIEnv *env = NULL;
    if (g_JavaVM->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return NULL;
    }
    return env;
}

jstring JNICALL
Java_com_example_media_MediaSt_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from eric";
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT jlong JNICALL Java_com_example_media_MediaSt_VideoDecCreate
        (JNIEnv *env, jobject obj)
{
    jlong handle = (jlong)100;

    LOG_INFO("VideoDecCreate %d\n", handle);
    return handle;
}

JNIEXPORT jint JNICALL Java_com_example_media_MediaSt_VideoDecDestroy
        (JNIEnv *env, jobject obj, jlong handle)
{
    LOG_INFO("VideoDecDestroy %d \n", handle);
    return 0;
}

JNIEXPORT jint JNICALL Java_com_example_media_MediaSt_VideoDecFlush
        (JNIEnv *env, jobject obj, jlong handle)
{
    LOG_INFO("ideoDecFlush %d \n", handle);
    return 0;
}

JNIEXPORT jint JNICALL Java_com_example_media_MediaSt_VideoDecWrite
        (JNIEnv *env, jobject obj, jlong handle, jbyteArray data, jint dataLen, jlong pts, jint timeoutMs)
{
    uint8_t *buf = (uint8_t *)(*env).GetByteArrayElements(data, 0);

    LOG_INFO("VideoDecWrite: %d %d (len=%d, timeout=%d, pts=%ld) \n", (int)buf[0], (int)buf[1], dataLen, timeoutMs, pts);

    (*env).ReleaseByteArrayElements(data, (jbyte *)buf, 0);
    return 0;
}

JNIEXPORT jint JNICALL Java_com_example_media_MediaSt_VideoDecRead
        (JNIEnv *env, jobject obj, jlong handle, jbyteArray out, jint outLen, jint timeoutMs)
{
    uint8_t *buf = (uint8_t *)(*env).GetByteArrayElements(out, 0);

    buf[0] = buf[1] = 30;
    LOG_INFO("VideoDecRead: (outLen=%d, timeout=%d) \n", outLen, timeoutMs);

    (*env).ReleaseByteArrayElements(out, (jbyte *)buf, 0);
    return 0;
}

static MediaLiveStreamListener *listener;

JNIEXPORT jlong JNICALL Java_com_example_media_MediaSt_LiveStreamCreate
        (JNIEnv *env, jobject obj)
{
    listener = new MediaLiveStreamListener(g_JavaVM, env, env->NewGlobalRef(obj));
    return (jlong)100;
}

JNIEXPORT jint JNICALL Java_com_example_media_MediaSt_LiveStreamDestroy
        (JNIEnv *env, jobject obj, jlong handle)
{
    delete listener;
    return 0;
}

JNIEXPORT jint JNICALL Java_com_example_media_MediaSt_LiveStreamWrite
        (JNIEnv *env, jobject obj, jlong handle, jbyteArray data, jint dataLen, jlong pts, jint timeoutMs)
{
    uint8_t *buf = (uint8_t *)(*env).GetByteArrayElements(data, 0);

    LOG_INFO("LiveStreamWrite: (outLen=%d, timeout=%d) \n", dataLen, timeoutMs);

    (*env).ReleaseByteArrayElements(data, (jbyte *)buf, 0);
    return 0;
}

void *test_thread(void *arg)
{
    uint8_t buf[2] = {5,6};
    listener->onFrameCallback(buf, 2, 1080, 720, 10, 21);
    return NULL;
}
static pthread_t tid;

JNIEXPORT jint JNICALL Java_com_example_media_MediaSt_LiveStreamStartRecord
        (JNIEnv *env, jobject obj, jlong handle, jstring filename)
{
    const char *str = (*env).GetStringUTFChars(filename, 0);
    if (!str) {
        LOG_ERROR("fatal error: str is null \n");
        return -1;
    }
    LOG_INFO("start record (%s) \n", str);
    pthread_create(&tid, NULL, test_thread, NULL);

    (*env).ReleaseStringUTFChars(filename, str);
    return 0;
}

JNIEXPORT jint JNICALL Java_com_example_media_MediaSt_LiveStreamStopRecord
        (JNIEnv *env, jobject obj, jlong handle)
{
    pthread_join(tid, NULL);
    return 0;
}

JNIEXPORT jint JNICALL Java_com_example_media_MediaSt_VideoExtractFrame
        (JNIEnv *env, jobject obj, jstring videoFile, jstring outputPath, jint num, jint hwEnable)
{
    const char *srcFilename = (*env).GetStringUTFChars(videoFile, 0);
    const char *dstPath = (*env).GetStringUTFChars(outputPath, 0);

    LOG_INFO("extract: %s %s \n", srcFilename, dstPath);

    (*env).ReleaseStringUTFChars(videoFile, srcFilename);
    (*env).ReleaseStringUTFChars(outputPath, dstPath);
    return 0;
}
