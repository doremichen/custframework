/**
 * File: com_cust_server_SpecialEffectsService.cpp
 * Breif: This is jni core implementation
 *
 * Name: Adam chen
 * Date: 
 */

#define LOG_NDEBUG 0
#define LOG_TAG "libcust_jni"

#include "jni.h"
#include <nativehelper/JNIHelp.h>

#include <cutils/log.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <CustClient.h>


#define FIND_CLASS(var, className) \
        var = env->FindClass(className); \
        LOG_FATAL_IF(! var, "Unable to find class " className);

#define GET_METHOD_ID(var, clazz, methodName, fieldDescriptor) \
        var = env->GetMethodID(clazz, methodName, fieldDescriptor); \
        LOG_FATAL_IF(! var, "Unable to find method " methodName);

#define GET_FIELD_ID(var, clazz, fieldName, fieldDescriptor) \
        var = env->GetFieldID(clazz, fieldName, fieldDescriptor); \
        LOG_FATAL_IF(! var, "Unable to find field " fieldName);

using namespace android;

static const char* const kClassName =
        "com/cust/server/SpecialEffectsService";
        
static CustClient* pClient = NULL;

static struct {
    /**
        // for the field of java layer
        jfieldID value;
    */
    // for the method of java layer
    jmethodID nativeCallBack;
} gJavaClassInfo;

/**
 * Used to notify java layer
 */
 static void
 notifyJavaLayer(JNIEnv *env, const char* str) {
     ALOGV("[%s] enter\n", __FUNCTION__);
     jclass clazz;
     jstring msgStr = env->NewStringUTF(str);
     if (!msgStr) {
            ALOGE("[%s]: Out of memory!!!\n", __FUNCTION__);
            return; // out of memory error
     }
     
     FIND_CLASS(clazz, "com/cust/server/SpecialEffectsService");
     
     jobject thiz = env -> AllocObject(clazz);
     
     ALOGV("[%s] invoke java method\n", __FUNCTION__);
     // invoke java method
     env -> CallVoidMethod(thiz, gJavaClassInfo.nativeCallBack, msgStr);
 }


/*
 * The method below are not thread-safe and not intended to be 
 */
static jint
special_effects_init(JNIEnv *env, jclass clazz)
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    jint ret;
    
    if(pClient == NULL) {
        pClient = new CustClient;
    }    
    
    ret = pClient->init_module();
    
    notifyJavaLayer(env, "initialize Successful...");
    
    return ret;
}

static jint
special_effects_on(JNIEnv *env, jclass clazz)
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    jint ret;
    
    ret = pClient->turn_on_lcd(); 
    
    notifyJavaLayer(env, "turn on screen light...");
    
    return ret;
}

static jint
special_effects_off(JNIEnv *env, jclass clazz)
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    jint ret;
    
    ret = pClient->turn_off_lcd(); 
    
    notifyJavaLayer(env, "turn off screen light...");
    
    return ret;
}

static void
special_effects_release(JNIEnv *env, jclass clazz)
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    if(pClient != NULL) {
        delete pClient;
        pClient = NULL;  
    }
    
    notifyJavaLayer(env, "release Successful...");
}

static JNINativeMethod gMethods[] = {
    {"_special_effects_init",     "()I", (void*) special_effects_init },
    {"_special_effects_on", "()I", (void*) special_effects_on },
    {"_special_effects_off", "()I", (void*) special_effects_off },
    {"_special_effects_release", "()V", (void*) special_effects_release }
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


