/**
 * File: com_med_server_endoscope_EndoScopeDeviceManager.cpp
 * Breif: Endoscope jni
 *
 * Name: 
 * Date: 
 */

#define LOG_NDEBUG 0
#define LOG_TAG "libendoscope_jni"

#include "jni.h"
#include <nativehelper/JNIHelp.h>

#include <cutils/log.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <NativeEndoscopeClient.h>

using namespace android;

#define FIND_CLASS(var, className) \
        var = env->FindClass(className); \
        LOG_FATAL_IF(! var, "Unable to find class " className);

#define GET_METHOD_ID(var, clazz, methodName, fieldDescriptor) \
        var = env->GetMethodID(clazz, methodName, fieldDescriptor); \
        LOG_FATAL_IF(! var, "Unable to find method " methodName);

#define GET_FIELD_ID(var, clazz, fieldName, fieldDescriptor) \
        var = env->GetFieldID(clazz, fieldName, fieldDescriptor); \
        LOG_FATAL_IF(! var, "Unable to find field " fieldName);

static const char* const kClassName =
        "com/med/server/endoscope/EndoScopeDeviceManager";
        
static NativeEndoscopeClient* pClient = nullptr;

JavaVM *gJvm = nullptr;  //Get gJvm from jni main thread use env->GetJavaVM(&gJvm);
jobject gObj = nullptr;  //Where the java function exist (some Activity).


static struct {
    /**
        // for the field of java layer
        jfieldID value;
    */
    // for the method of java layer
    jmethodID nativeCallBack;
} gJavaClassInfo;

const char* getCharFromString(JNIEnv* env, jstring string){
    if(string == nullptr)
        return nullptr;

    return  env->GetStringUTFChars(string ,0);
}


/**
 * Used to notify java layer
 */
static void
notifyJavaLayer(const char* str, jboolean isMainThread) 
{
    ALOGV("[%s] enter: %s\n", __FUNCTION__, str);
    //Get env in thread function and attach the env
    ALOGI("Get env in thread function and attach the env");
    JNIEnv *env;
    if(gJvm->AttachCurrentThread(&env, NULL) != JNI_OK) {
        ALOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
    }
    
    jstring msgStr = env->NewStringUTF(str);
    if (!msgStr) {
           ALOGE("[%s]: Out of memory!!!\n", __FUNCTION__);
           return; // out of memory error
    }
    
    ALOGI("msgStr: %s\n", getCharFromString(env, msgStr));

    ALOGV("[%s] invoke java method \n", __FUNCTION__);
    // invoke java method
    env -> CallVoidMethod(gObj, gJavaClassInfo.nativeCallBack, msgStr);
    
    //Detach thread and release related resource
    ALOGI("Detach thread and release related resource if needed");
    if (isMainThread == JNI_FALSE) {
        if(gJvm->DetachCurrentThread() != JNI_OK) {
            ALOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
        }
    }
}

/*
 * The method below are not thread-safe and not intended to be 
 */
static jint
getEndoScopeId(JNIEnv *env, jobject thiz)
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    jint ret = 0;
    
    // for callback java layer
    gObj  = env->NewGlobalRef(thiz);
    
    if(pClient == nullptr) {
        ALOGE("No native endoscope client hanlder!!!");
        notifyJavaLayer("No native endoscope client hanlder!!!", JNI_TRUE);
        return -1;
    }    
    
    ret = pClient->getScopeId();
    
    notifyJavaLayer("getEndoScopeId Successful...", JNI_TRUE);
    
    // release reference object
    env->DeleteGlobalRef(gObj);
    
    return ret;
}

static JNINativeMethod gMethods[] = {
    {"_getEndoScopeId",     "()I", (void*) getEndoScopeId },
};


static int registerMethods(JNIEnv* env) 
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    jclass clazz;

    /* look up the class */
    clazz = env->FindClass(kClassName);
    if (clazz == NULL) {
        ALOGE("[%s]: Can't find class %s\n", __FUNCTION__,  kClassName);
        return -1;
    }

    /* register all the methods */
    if (env->RegisterNatives(clazz, gMethods,
            sizeof(gMethods) / sizeof(gMethods[0])) != JNI_OK)
    {
        ALOGE("[%s]: Failed registering methods for %s\n", __FUNCTION__, kClassName);
        return -1;
    }

    // get method id of java class
    GET_METHOD_ID(gJavaClassInfo.nativeCallBack,
            clazz, "notifyFromNative", "(Ljava/lang/String;)V");

    /* fill out the rest of the ID cache */
    return 0;
}


// ----------------------------------------------------------------------------

/*
 * This is called by the VM when the shared library is first loaded.
 */
jint JNI_OnLoad(JavaVM* vm, void* reserved) 
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    JNIEnv* env = NULL;
    jint result = -1;
    
    gJvm = vm;
    
    // instance native endoscope client handler
    pClient = new NativeEndoscopeClient;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("[%s]: ERROR: GetEnv failed\n", __FUNCTION__);
        goto bail;
    }
    assert(env != NULL);

    if (registerMethods(env) != 0) {
        ALOGE("[%s]: ERROR: PlatformLibrary native registration failed\n", __FUNCTION__);
        goto bail;
    }

    /* success -- return valid version number */
    result = JNI_VERSION_1_6;

bail:
    return result;
}


