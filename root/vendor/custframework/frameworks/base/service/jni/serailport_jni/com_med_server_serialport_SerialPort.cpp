/**
 * File: com_med_server_serialport_SerialPort.cpp
 * Breif: Serialport jni
 *
 * Name: 
 * Date: 
 */

#define LOG_NDEBUG 0
#define LOG_TAG "libserailport_jni"

#include "jni.h"
#include <nativehelper/JNIHelp.h>

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cutils/log.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

//#define TEST


#define FIND_CLASS(var, className) \
        var = env->FindClass(className); \
        LOG_FATAL_IF(! var, "Unable to find class " className);

#define GET_METHOD_ID(var, clazz, methodName, fieldDescriptor) \
        var = env->GetMethodID(clazz, methodName, fieldDescriptor); \
        LOG_FATAL_IF(! var, "Unable to find method " methodName);

#define GET_FIELD_ID(var, clazz, fieldName, fieldDescriptor) \
        var = env->GetFieldID(clazz, fieldName, fieldDescriptor); \
        LOG_FATAL_IF(! var, "Unable to find field " fieldName);

// message commands length
#define MAX_datas_SIZE 1024

volatile jboolean gRunning = JNI_FALSE;

int gFd = -1;

char gRead_buffer[MAX_datas_SIZE] = {0};

JNIEnv *gEnv = nullptr;
JavaVM *gJvm = nullptr;  //Get gJvm from jni main thread use env->GetJavaVM(&gJvm);
jobject gObj = nullptr;  //Where the java function exist (some Activity).


static const char* const kClassName =
        "com/med/server/serialport/SerialPort";

static struct {
    /**
        // for the field of java layer
        jfieldID value;
    */
    // for the method of java layer
    jmethodID nativeCallBack;
} gJavaClassInfo;

//*****************************************************************************
// FUNCTION NAME: getCharFromString
// DESCRIPTION:   show jstring info
//
// ARGs:          JNIEnv* env, jstring string
//
// RETURNS:       const char*
//*****************************************************************************
const 
char* getCharFromString(JNIEnv* env, jstring string){
    if(string == nullptr)
        return nullptr;

    return  env->GetStringUTFChars(string ,0);
}

//*****************************************************************************
// FUNCTION NAME: getBaudrate
// DESCRIPTION:   set baudrate
//
// ARGs:          int
//
// RETURNS:       speed_t
//*****************************************************************************
static 
speed_t getBaudrate(jint baudrate)
{
    switch(baudrate) {
    case 0: return B0;
    case 50: return B50;
    case 75: return B75;
    case 110: return B110;
    case 134: return B134;
    case 150: return B150;
    case 200: return B200;
    case 300: return B300;
    case 600: return B600;
    case 1200: return B1200;
    case 1800: return B1800;
    case 2400: return B2400;
    case 4800: return B4800;
    case 9600: return B9600;
    case 19200: return B19200;
    case 38400: return B38400;
    case 57600: return B57600;
    case 115200: return B115200;
    case 230400: return B230400;
    case 460800: return B460800;
    case 500000: return B500000;
    case 576000: return B576000;
    case 921600: return B921600;
    case 1000000: return B1000000;
    case 1152000: return B1152000;
    case 1500000: return B1500000;
    case 2000000: return B2000000;
    case 2500000: return B2500000;
    case 3000000: return B3000000;
    case 3500000: return B3500000;
    case 4000000: return B4000000;
    default: return -1;
    }
}

//*****************************************************************************
// FUNCTION NAME: isready
// DESCRIPTION:   check the status of uart IO
//
// ARGs:          int
//
// RETURNS:       int
//*****************************************************************************
static 
int isready(int fd)
{
    ALOGV("[%s] +++\n", __FUNCTION__);
    int readState = -1;
    fd_set readFd_set;

    FD_ZERO(&readFd_set);
    FD_SET(fd,&readFd_set);

    readState = select(fd+1, &readFd_set, NULL, NULL, NULL);

    if(readState > 0) {        
        return FD_ISSET(fd, &readFd_set) ? 1 : 0;          
    }
    
    
    if( readState == 0) {
        ALOGE("[%s]: No data from devices.\n", __FUNCTION__);
    }        
    else {    
        ALOGE("[%s]: Fail to select in readline. error: %s\n", __FUNCTION__, strerror(errno));  
    }
    ALOGV("[%s] xxx readState: %d\n", __FUNCTION__, readState);
    return readState;
}

//*****************************************************************************
// FUNCTION NAME: notifyJavaLayer
// DESCRIPTION:   Call back to java layer
//
// ARGs:          JNIEnv *env, const char* str
//
// RETURNS:
//*****************************************************************************
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


//*****************************************************************************
//thread body
//*****************************************************************************
void
testNotify(const char* str, size_t size)
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    memcpy(gRead_buffer, str, size);
    ALOGV("gRead_buffer: %s\n", gRead_buffer);
    // notify java layer
    notifyJavaLayer(gRead_buffer, JNI_FALSE);
    ALOGV("[%s] exit\n", __FUNCTION__);
}

void*
serialPortSession(void* arg) {
    ALOGV("[%s] enter\n", __FUNCTION__);
    int readCount = 0;
    
    while (gRunning == JNI_TRUE) {
        ALOGV("Hi!!!!");
        // clear buf
        //Set the commands data to zero
        memset(gRead_buffer, 0x00, sizeof(gRead_buffer));
        
#ifdef TEST
        const char* test = "123456789abcdef";
        testNotify(test, strlen(test));
#else
        if (isready(gFd) == 1) {
            // read command from tty
            readCount = read(gFd, &gRead_buffer, sizeof(gRead_buffer));
            
            if(readCount == -1) {
                ALOGE("[%s]: read Comm COM port error! error: %s\n", __FUNCTION__, strerror(errno));
                continue;  
            }
            
            // put string in native callback
            if(readCount > 0) {                    
                ALOGV("[%s]: COMM COM port have data<%d>: %x\n", __FUNCTION__, readCount, *gRead_buffer);
                notifyJavaLayer(gRead_buffer, JNI_FALSE);
            }
        }
#endif
        sleep(1);
    }
    
    // release reference object
    gEnv->DeleteGlobalRef(gObj);
    
    ALOGV("serial port session task finish!!!");
    pthread_exit(NULL);
}

static void
startSessionTask()
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    pthread_t thread_id;
    ALOGD("start serial port session task: Serial_port_task!!!");
    gRunning = JNI_TRUE;
    int ret = pthread_create(&thread_id, NULL, serialPortSession, NULL);
    if (ret != 0) {
        ALOGE("Failed to create thread\n");
        return;
    }
    ALOGV("[%s] exit\n", __FUNCTION__);
}

/*
 * 
 */
static void
openSerialPort(JNIEnv *env, jobject thiz, jstring path, jint baudrate, jint flags)
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    speed_t speed = {};
    gEnv = env;
    // for callback java layer
    gObj = env->NewGlobalRef(thiz);
    
#ifdef TEST
    startSessionTask();
#else
    /* Check arguments */
    speed = getBaudrate(baudrate);
    if (speed == -1) {
        /* TODO: throw an exception */
        ALOGE("Invalid baudrate");
        return;
    }
    
    /* Opening device */
    jboolean iscopy = false;
    const char *path_utf = env ->GetStringUTFChars(path, &iscopy);
//    LOGD("Opening serial port %s with flags 0x%x", path_utf, O_RDWR | O_NOCTTY | O_NONBLOCK | flags);
//    gFd = open(path_utf, O_RDWR | O_NOCTTY | O_NONBLOCK | flags);
    ALOGD("Opening serial port %s with flags 0x%x", path_utf, O_RDWR | flags);
    gFd = open(path_utf, O_RDWR | flags);
    ALOGD("open() gFd = %d", gFd);
    env ->ReleaseStringUTFChars(path, path_utf);
    if (gFd == -1)
    {
        /* Throw an exception */
        ALOGE("Cannot open port");
        /* TODO: throw an exception */
        return;
    }
    
    
    /* Configure device */
    struct termios cfg;
    ALOGD("Configuring serial port");
    if (tcgetattr(gFd, &cfg))
    {
        ALOGE("tcgetattr() failed");
        close(gFd);
        /* TODO: throw an exception */
        return;
    }

    cfmakeraw(&cfg);
//    cfg.c_iflag = IGNBRK;
//    cfg.c_oflag = 0;
//    cfg.c_cflag |= (CREAD | CLOCAL| CS8); //data Bit: 8 bits
//    cfg.c_cflag &= ~PARENB; //Parity: None
//    cfg.c_cflag &= ~CSTOPB; //Stop Bits: 1
    cfsetispeed(&cfg, speed);
    cfsetospeed(&cfg, speed);

    if (tcsetattr(gFd, TCSANOW, &cfg))
    {
        ALOGE("tcsetattr() failed");
        close(gFd);
        /* TODO: throw an exception */
        return;
    }
    
    // start session thread
    startSessionTask();
#endif
    ALOGV("[%s] exit\n", __FUNCTION__);
}

static void
closeSerialPort(JNIEnv *env, jobject thiz) 
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    // stop session
    gRunning = JNI_FALSE;
    if (gFd != -1) {
        close(gFd);
    }
    // release globle reference
    env->DeleteGlobalRef(gObj);
    
    ALOGV("[%s] exit\n", __FUNCTION__);
}

static void
send(JNIEnv *env, jobject thiz, jbyteArray data)
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    
    if (gFd == -1) {
        ALOGE("No serial port file descriptor!!! please check whether open port first or not!\n");
        return;
    }
    
    int bytes_size = env->GetArrayLength(data);
    jbyte* commands = env->GetByteArrayElements(data, 0);
    // to char*
    unsigned char* write_buffer = reinterpret_cast<unsigned char*>(commands);
    
    // write data to serial port
    int number_of_write = 0;
    
    do {
        number_of_write = write(gFd, write_buffer, bytes_size);
        
        if (number_of_write < 0) {
            ALOGE("write fail!!!: error: %s\n", strerror(errno));
            break;
        }
        
        // update write satrt position
        write_buffer += number_of_write;
        
        // remaining length
        bytes_size -= number_of_write;
    } while(bytes_size > 0);
    
    
    // release resource
    env->ReleaseByteArrayElements(data, commands, 0);
    
    ALOGV("[%s] exit\n", __FUNCTION__);
}

static void
foo(JNIEnv *env, jobject thiz)
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    
    ALOGV("[%s] exit\n", __FUNCTION__);
}

static JNINativeMethod gMethods[] = {
    {"native_openSerialPort", "(Ljava/lang/String;II)V", (void*) openSerialPort},
    {"native_closeSerialPort", "()V", (void*) closeSerialPort},
    {"native_send", "([B)V", (void*) send},
    {"native_foo", "()V", (void*) foo},
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


