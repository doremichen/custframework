
#define LOG_NDEBUG 0
#define LOG_TAG "libcust_jni"

#include "jni.h"
#include "JNIHelp.h"

#include <cutils/log.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <CustClient.h>

using namespace android;

static const char* const kClassName =
        "com/cust/server/SpecialEffectsService";
        
static CustClient* pClient = NULL;        

/*
 * The method below are not thread-safe and not intended to be 
 */
static jint
special_effects_init(JNIEnv *env, jclass clazz)
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    
    if(pClient == NULL) {
        pClient = new CustClient;   
    }    
    
    pClient->init_module();
    
    return 0;
}

static jint
special_effects_on(JNIEnv *env, jclass clazz)
{
	ALOGV("[%s] enter\n", __FUNCTION__);
	 
	pClient->turn_on_lcd(); 
	 
	return 0;   
}

static jint
special_effects_off(JNIEnv *env, jclass clazz)
{
	 ALOGV("[%s] enter\n", __FUNCTION__);
	 
	 pClient->turn_off_lcd(); 
	 
	return 0; 
	  
}

static jint
special_effects_release(JNIEnv *env, jclass clazz)
{
	 ALOGV("[%s] enter\n", __FUNCTION__);
	 if(pClient != NULL) {
	    delete pClient;
	    pClient = NULL;  
	 }   
	 
	return 0; 
	  
}

static JNINativeMethod gMethods[] = {
    {"_special_effects_init",     "()I", (void*) special_effects_init },
    {"_special_effects_on", "()I", (void*) special_effects_on },
    {"_special_effects_off", "()I", (void*) special_effects_off },
    {"_special_effects_release", "()I", (void*) special_effects_release }
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
    result = JNI_VERSION_1_4;

bail:
    return result;
}


