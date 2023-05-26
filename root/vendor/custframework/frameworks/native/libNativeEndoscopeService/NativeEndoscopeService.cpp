/**
 * File: NativeEndoscopeService.cpp
 * Breif: Implement native Service 
 *
 * Name: 
 * Date: 
 */

#define LOG_NDEBUG 0
#define LOG_TAG "NativeEndoscopeService"
#include <cutils/log.h>

#include "CommonEndoscopeUtils.h"
#include "NativeEndoscopeService.h"
#include <binder/IServiceManager.h>  
#include <binder/IPCThreadState.h>
#include <binder/Status.h>
#include <utils/Errors.h>
#include <errno.h>

#include <endoscopehal/endoscopehal.h> 


//#define SYSTEM_HAL

namespace android {

#ifdef SYSTEM_HAL
    static endoscope_device_t *endoscopeDevice = nullptr;
#endif
    /**
     * add native endoscope service in service list via system service
     */
    int NativeEndoscopeService::instantiate()  
    {  
        ALOGV("NativeEndoscopeService instantiate");  
        //  
        int ret = 0;  
        
        auto status = defaultServiceManager()->addService(String16("native.endoscope.service"), 
                            new NativeEndoscopeService());
        
        if (status != OK) {
            ALOGE("Could not self register servicemanager");
            return EINVAL;
        }
        
        
#ifdef SYSTEM_HAL
        //get endoscope hal module and device
        endoscope_module_t const * module;
        if (hw_get_module(ENDOSCOPE_MODULE_ID, (const hw_module_t**)&module) == 0) {
            ALOGV("[%s]: hw_get_module \n", __FUNCTION__);
            if (endoscope_hal_open(&module->common, &endoscopeDevice) != 0) {
                 ALOGE("[%s]: endoscope hal open fail! \n", __FUNCTION__);
                 ret = EINVAL;  // Invalid argument
            }
             
        }
        ALOGV("NativeEndoscopeService: endoscopeDevice[%#x]\n", endoscopeDevice);
#endif
        ALOGV("NativeEndoscopeService instantiate: ret[%d]\n", ret); 
        return ret;  
    }  
  
    NativeEndoscopeService::NativeEndoscopeService()  
    {   
        ALOGV("NativeEndoscopeService created");
    }  
  
    NativeEndoscopeService::~NativeEndoscopeService()  
    {  
        ALOGV("NativeEndoscopeService destroyed");  
    }
 
 
    /**
     * Open hal module interface
     */
    int NativeEndoscopeService::getScopeId()
    {
        ALOGV("[%s] enter\n", __FUNCTION__);
        int ret = 0;  // Success
#ifdef SYSTEM_HAL
        if (endoscopeDevice == nullptr) {
            return -1;
        }
        
        ret = endoscopeDevice -> pGetSensorId(endoscopeDevice);
#endif
         
        ALOGV("[%s] done\n", __FUNCTION__);
        return ret;
            
    }
    
    /**
     * Recieve the request from binder driver
     */
    status_t NativeEndoscopeService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)  
    {  
        
        ALOGV("[%s]: code[%d]\n", __FUNCTION__, code);  
        switch(code)  
        {   
            case GET_SCOPE_ID: {  
                pid_t pid = data.readInt32();
                ALOGV("[%s]: pid[%d]\n", __FUNCTION__, pid);
                int ret = getScopeId();   // init android hal module
                reply->writeInt32(ret);  
                return NO_ERROR;
            }
            break;
            
                  
            default:  
                return BBinder::onTransact(code, data, reply, flags);
        }  
    }
    
      
}; //namespace  