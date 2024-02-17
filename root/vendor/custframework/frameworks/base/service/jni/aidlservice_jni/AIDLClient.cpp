/**
 * File: AIDLClient.cpp
 * Breif: Implement Client class
 *
 * Name: 
 * Date: 
 */

#define LOG_NDEBUG 0  
#define LOG_TAG "AIDLClient"
#include <cutils/log.h>

#include "CommonAIDLServiceUtils.h"
#include "AIDLClient.h"
  
namespace android  
{  
    sp<IAIDLService> AIDLClient::BpAIDLService = NULL;
    
    AIDLClient::AIDLClient()
    {
        ALOGV("[%s]: enter\n", __FUNCTION__);
        BpAIDLService = AIDLClient::getAIDLService();
        ALOGV("[%s]: BpAIDLService[%p]\n", __FUNCTION__, BpAIDLService.get());
    }
    
    AIDLClient::~AIDLClient()
    {
       ALOGV("[%s]: enter\n", __FUNCTION__);
       
       exit();     
            
    }
    
    /**
     * Clear Binder proxy
     */
    void AIDLClient::exit()
    {
         ALOGV("[%s]: enter\n", __FUNCTION__);
         if (BpAIDLService == nullptr) {
            ALOGE("BpAIDLService is null!!!");
            return;
         }
         
         BpAIDLService.clear();
         BpAIDLService = NULL;       
                        
    }
    
    /**
     * Send init request to binder driver
     */
    int AIDLClient::init_module()  
    {   
        ALOGV("[%s]: enter\n", __FUNCTION__);
        if (BpAIDLService == nullptr) {
            ALOGE("BpAIDLService is null!!!");
            return -1;
         }
        
        BpAIDLService -> init();
        return 0;  
    } 
    
    void AIDLClient::setAIDLServiceCallback(const sp<IAIDLServiceCallback>& callback)
    {
        ALOGV("[%s] enter\n", __FUNCTION__);
        ALOGV("callback [%p] \n", callback.get());
        if (BpAIDLService == nullptr) {
            ALOGE("BpAIDLService is null!!!");
            return;
         }
         
        BpAIDLService -> setCallback(callback);
        
        ALOGV("[%s] exit\n", __FUNCTION__);
    }
    
    void AIDLClient::testCallback() {
        ALOGV("[%s] enter\n", __FUNCTION__);
        if (BpAIDLService == nullptr) {
            ALOGE("BpAIDLService is null!!!");
            return;
         }
         
        BpAIDLService -> testCallback();
        
        ALOGV("[%s] exit\n", __FUNCTION__);
    }
    
    /**
     * Get binder proxy via system service
     */
    const sp<IAIDLService> AIDLClient::getAIDLService()  
    {  
        ALOGV("[%s] enter\n", __FUNCTION__);
        sp<IServiceManager> sm = defaultServiceManager();
        if(sm == nullptr) {
            ALOGE("failed to get service manager");
        return nullptr;
        }
        sp<IBinder> binder = sm->getService(String16(SERVICE_NAME));
        if(binder == nullptr) {
            ALOGE("failed to get IAIDLService");
            return nullptr;
        }
        
        sp<IAIDLService> service = interface_cast<IAIDLService>(binder);
        ALOGV("service: [%p]\n", service.get());
        ALOGV("[%s] exit\n", __FUNCTION__);
        return service;
    }
      
}; //namespace  
 