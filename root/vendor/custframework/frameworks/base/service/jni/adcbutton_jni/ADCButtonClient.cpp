/**
 * File: ADCButtonClient.cpp
 * Breif: Implement Client class
 *
 * Name: 
 * Date: 
 */

#define LOG_NDEBUG 0  
#define LOG_TAG "ADCButtonClient"
#include <cutils/log.h>

#include "CommonADCButtonUtils.h"
#include "ADCButtonClient.h"
  
namespace android  
{  
    sp<IADCButtonService> ADCButtonClient::BpADCButtonService = NULL;
    
    ADCButtonClient::ADCButtonClient()
    {
        ALOGV("[%s]: enter\n", __FUNCTION__);
        BpADCButtonService = ADCButtonClient::getADCButtonService();
        ALOGV("[%s]: BpADCButtonService[%p]\n", __FUNCTION__, BpADCButtonService.get());
    }
    
    ADCButtonClient::~ADCButtonClient()
    {
       ALOGV("[%s]: enter\n", __FUNCTION__);
       
       exit();     
            
    }
    
    /**
     * Clear Binder proxy
     */
    void ADCButtonClient::exit()
    {
         ALOGV("[%s]: enter\n", __FUNCTION__);
         if (BpADCButtonService == nullptr) {
            ALOGE("BpADCButtonService is null!!!");
            return;
         }
         
         BpADCButtonService.clear();
         BpADCButtonService = NULL;       
                        
    }
    
    /**
     * Send init request to binder driver
     */
    int ADCButtonClient::init_module()  
    {   
        ALOGV("[%s]: enter\n", __FUNCTION__);
        if (BpADCButtonService == nullptr) {
            ALOGE("BpADCButtonService is null!!!");
            return -1;
         }
        
        BpADCButtonService -> init();
        return 0;  
    } 
    
    void ADCButtonClient::setADCButtonCallback(const sp<IADCButtonCallback>& callback)
    {
        ALOGV("[%s] enter\n", __FUNCTION__);
        ALOGV("callback [%p] \n", callback.get());
        if (BpADCButtonService == nullptr) {
            ALOGE("BpADCButtonService is null!!!");
            return;
         }
         
        BpADCButtonService -> setCallback(callback);
        
        ALOGV("[%s] exit\n", __FUNCTION__);
    }
    
    /**
     * Get binder proxy via system service
     */
    const sp<IADCButtonService> ADCButtonClient::getADCButtonService()  
    {  
        ALOGV("[%s] enter\n", __FUNCTION__);
        sp<IServiceManager> sm = defaultServiceManager();
        if(sm == nullptr) {
            ALOGE("failed to get service manager");
        return nullptr;
        }
        sp<IBinder> binder = sm->getService(String16(SERVICE_NAME));
        if(binder == nullptr) {
            ALOGE("failed to get IADCButtonService");
            return nullptr;
        }
        
        sp<IADCButtonService> service = interface_cast<IADCButtonService>(binder);
        ALOGV("service: [%p]\n", service.get());
        ALOGV("[%s] exit\n", __FUNCTION__);
        return service;
    }
      
}; //namespace  
 