// File: CustManager.cpp
#define LOG_NDEBUG 0  
#define LOG_TAG "CustManager"
#include <cutils/log.h>

#include <sys/types.h>  
#include <unistd.h>  
#include <grp.h>  
#include <binder/IPCThreadState.h>  
#include <binder/ProcessState.h>  
#include <binder/IServiceManager.h>  
#include <utils/Log.h>   
#include <NativeService.h>
  
using namespace android;  
  
int main(int argc, char** argv)  
{  
    sp<ProcessState> proc(ProcessState::self());
    sp<IServiceManager> sm = defaultServiceManager();  
    ALOGI("ServiceManager: %p", sm.get());  
    
    NativeService::instantiate(); 
    
    ProcessState::self()->startThreadPool();  
    IPCThreadState::self()->joinThreadPool();  
    return 0;  
}  