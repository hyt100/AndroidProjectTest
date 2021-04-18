#pragma  once
#include <jni.h>
#include <stdint.h>
#include <string>
#include <android/log.h>

class MediaLiveStreamListener {
 public:
    MediaLiveStreamListener(JavaVM* vm, JNIEnv *env, jobject obj, int type = 1)
    {
        type_ = type;
        jvm_ = vm;
        jenv_ = env;
        jobj_ = obj;
        jclass clz = (*env).GetObjectClass(obj);
        if (!clz)
        {
            //LOGE("get jclass wrong");
            return;
        }
        jmid_framecallbak_ = env->GetMethodID(clz, "LiveStreamOnCallbackFrame", "([BIIJI)V");
        if(!jmid_framecallbak_)
        {
            //LOGE("get jmethodID wrong");
            return;
        }
    }

    ~MediaLiveStreamListener()
    {
    }

    void onFrameCallback(uint8_t *buf, int size, int width, int height, int64_t pts, int format)
    {
        if(type_ == 0)
        {
            jbyte *data = (jbyte *)buf;
            jbyteArray array = jenv_->NewByteArray(size);
            jenv_->SetByteArrayRegion(array, 0, size, data);

            jenv_->CallVoidMethod(jobj_, jmid_framecallbak_, array, width, height, pts, format);
            jenv_->DeleteLocalRef(array);
        }
        else if(type_ == 1)
        {
            JNIEnv *env;
            jvm_->AttachCurrentThread(&env, 0);

            jbyte *data = (jbyte *)buf;
            jbyteArray array = env->NewByteArray(size);
            env->SetByteArrayRegion(array, 0, size, data);

            env->CallVoidMethod(jobj_, jmid_framecallbak_, array, width, height, pts, format);
            env->DeleteLocalRef(array);

            jvm_->DetachCurrentThread();
        }
    }

 private:
    JavaVM* jvm_;
    JNIEnv *jenv_;
    jobject jobj_;
    jmethodID jmid_framecallbak_;
    int type_;
};
