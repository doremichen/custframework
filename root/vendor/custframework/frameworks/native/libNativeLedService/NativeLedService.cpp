/**
 * File: NativeLedService.cpp
 * Breif: Implement native Service 
 *
 * Name: 
 * Date: 
 */

#define LOG_NDEBUG 0
#define LOG_TAG "NativeLedService"
#include <cutils/log.h>

#include "CommonLedUtils.h"
#include "NativeLedService.h"
#include <binder/IServiceManager.h>  
#include <binder/IPCThreadState.h> 
#include <errno.h>

namespace android {
    
    /**
     * add cust native service in service list via system service
     */
    int NativeLedService::instantiate()  
    {  
        ALOGV("NativeLedService instantiate");  
        //  
        int r = defaultServiceManager()->addService(String16("native.led.service"), new NativeLedService());  
        ALOGV("NativeLedService r = %d/n", r);
 
        return r;  
    }  
  
    NativeLedService::NativeLedService()  
    {   
        ALOGV("NativeLedService created");
                       
    }  
  
    NativeLedService::~NativeLedService()  
    {  
        ALOGV("NativeLedService destroyed");  
    }  
 
 
    /**
     * Open hal module interface
     */
    int NativeLedService::module_init()
    {
        ALOGV("[%s] enter\n", __FUNCTION__);
        int ret = 0;  // Success
        
        ALOGV("[%s] done\n", __FUNCTION__);
        return ret;
            
    }
    
    /**
     * Recieve the request from binder driver
     */
    status_t NativeLedService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)  
    {  
        
        ALOGV("[%s]: code[%d]\n", __FUNCTION__, code);  
        switch(code)  
        {   
            case INIT_NATIVE_LED_MODULE: {  
                pid_t pid = data.readInt32();
                ALOGV("[%s]: pid[%d]\n", __FUNCTION__, pid);
                int ret = module_init();   // init android hal module
                reply->writeInt32(ret);  
                return NO_ERROR;
            }
            break;
                  
            default:  
                return BBinder::onTransact(code, data, reply, flags);
        }  
    }
    
      
}; //namespace  