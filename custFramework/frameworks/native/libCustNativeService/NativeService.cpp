/**
 * File: NativeService.cpp
 * Breif: Implement native Service 
 *
 * Name: Adam chen
 * Date: 
 */

#define LOG_NDEBUG 0
#define LOG_TAG "NativeService"
#include <cutils/log.h>
  
#include "NativeService.h"
#include <binder/IServiceManager.h>  
#include <binder/IPCThreadState.h> 
#include <errno.h>

#include <custhal/custhal.h> 

namespace android {
    
    static cust_device_t *custDevice = 0;
    
    /**
     * add cust native service in service list via system service
     */
    int NativeService::instantiate()  
    {  
        ALOGV("NativeService instantiate");  
        //  
        int r = defaultServiceManager()->addService(String16("cust.native.service"), new NativeService());  
        ALOGV("NativeService r = %d/n", r);
 
        return r;  
    }  
  
    NativeService::NativeService()  
    {   
        ALOGV("NativeService created");
                       
    }  
  
    NativeService::~NativeService()  
    {  
        ALOGV("NativeService destroyed");  
    }  
 
 
    /**
     * Open hal module interface
     */
    int NativeService::module_init()
    {
        ALOGV("[%s] enter\n", __FUNCTION__);
        int ret = 0;  // Success
        
         //get android hal module and device
        cust_module_t const * module;
        if (hw_get_module(CUST_HARDWARE_MODULE_ID, (const hw_module_t**)&module) == 0) {
            ALOGV("[%s]: hw_get_module \n", __FUNCTION__);
            if (cust_effects_open(&module->common, &custDevice) != 0) {
                 ALOGE("[%s]: special_effects_open fail! \n", __FUNCTION__);
                 ret = EINVAL;  // Invalid argument
            }
             
        } 
        ALOGV("[%s] done\n", __FUNCTION__);
        return ret;
            
    }        
 
    /**
     * Recieve the request from binder driver
     */
    status_t NativeService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)  
    {  
        
        ALOGV("[%s]: code[%d]\n", __FUNCTION__, code);  
        switch(code)  
        {   
            case INIT_NATIVE_MODULE: {  
                pid_t pid = data.readInt32();  
                int ret = module_init();   // init android hal module
                reply->writeInt32(ret);  
                return NO_ERROR;
            }
            break;
            
            
            case TURN_ON_EFFECT: {  
                pid_t pid = data.readInt32();  
                int ret = custDevice->cust_effects_on(custDevice);   // turn on led
                reply->writeInt32(ret);  
                return NO_ERROR;
            }
            break; 
                
            case TURN_OFF_EFFECT: {  
                pid_t pid = data.readInt32();  
                int ret = custDevice->cust_effects_off(custDevice);   // turn off led
                reply->writeInt32(ret);  
                return NO_ERROR;
            }
            break;
                  
            default:  
                return BBinder::onTransact(code, data, reply, flags);
        }  
    }
    
      
}; //namespace  