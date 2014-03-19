// File: NativeService.cpp
#define LOG_NDEBUG 0
#define LOG_TAG "NativeService"
#include <cutils/log.h>
  
#include "NativeService.h"
#include <binder/IServiceManager.h>  
#include <binder/IPCThreadState.h> 

#include <custdroid/custhal.h> 

#define HAPTIC_HARDWARE_MODULE_ID "haptic"   //test

namespace android {  
    
    static cust_device_t *custDevice = 0;
  
    static struct sigaction oldact;  
    static pthread_key_t sigbuskey;  
      
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
        pthread_key_delete(sigbuskey);  
        ALOGV("NativeService destroyed");  
    }  
 
 
    int NativeService::module_init()
    {
        ALOGV("[%s] enter\n", __FUNCTION__);
        int ret = 1;
        
         //get android hal module and device
        cust_module_t const * module;
        if (hw_get_module(CUST_HARDWARE_MODULE_ID, (const hw_module_t**)&module) == 0) {
        	ALOGV("[%s]: hw_get_module \n", __FUNCTION__);
            if (cust_effects_open(&module->common, &custDevice ) != 0) {
                 ALOGE("[%s]: special_effects_open fail! \n", __FUNCTION__);
                 ret = 0;
            }
             
        } 
        ALOGV("[%s] done\n", __FUNCTION__);
        return ret;      
            
    }        
 
 
    status_t NativeService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)  
    {  
        
        ALOGV("[%s]: code[%d]\n", __FUNCTION__, code);  
        switch(code)  
        {   
            case INIT_NATIVE_MODULE: {  
                pid_t pid = data.readInt32();  
                int ret = module_init();   //init android hal module               
                reply->writeInt32(ret);  
                return NO_ERROR;
            }
                break;
            
            
            case TURN_ON_EFFECT: {  
                pid_t pid = data.readInt32();  
                int ret = custDevice->cust_effects_on(custDevice);   //turn on led               
                reply->writeInt32(ret);  
                return NO_ERROR;
            }
                break; 
                
            case TURN_OFF_EFFECT: {  
                pid_t pid = data.readInt32();  
                int ret = custDevice->cust_effects_off(custDevice);   //turn off led                 
                reply->writeInt32(ret);  
                return NO_ERROR;
            }
                break;         
                  
            default:  
                return BBinder::onTransact(code, data, reply, flags);  
        }  
    }
    
      
}; //namespace  