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
#include <NativeEndoscopeService.h>
#include <NativeLedService.h>
#include <ADCButtonService.h>

#define noinline __attribute__((__noinline__))
#define __unused __attribute__((__unused__))

using namespace android;
  
int main(int argc __unused, char* argv[] __unused)
{  
    ALOGI("Cust Manager start...\n");
    sp<ProcessState> proc(ProcessState::self());
    sp<IServiceManager> sm = defaultServiceManager();  
    ALOGI("ServiceManager: %p", sm.get());  
    
    NativeService::instantiate(); 
    NativeEndoscopeService::instantiate();
    NativeLedService::instantiate();
    ADCButtonService::instantiate();
    
    ProcessState::self()->startThreadPool();  
    IPCThreadState::self()->joinThreadPool();  
    return 0;  
}  