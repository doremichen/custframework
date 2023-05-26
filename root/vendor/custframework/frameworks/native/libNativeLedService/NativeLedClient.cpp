/**
 * File: NativeLedClient.cpp
 * Breif: Implement Client class
 *
 * Name: 
 * Date: 
 */

#define LOG_NDEBUG 0  
#define LOG_TAG "NativeLedClient"
#include <cutils/log.h>

#include "CommonLedUtils.h"
#include "NativeLedClient.h"
  
namespace android  
{  
    sp<IBinder> NativeLedClient::BpCustNativeService = NULL;
    
    NativeLedClient::NativeLedClient()
    {
        ALOGV("[%s]: enter\n", __FUNCTION__);
        getCustNativeService();
    }
    
    NativeLedClient::~NativeLedClient()
    {
       ALOGV("[%s]: enter\n", __FUNCTION__);
       
       exit();     
            
    }
    
    /**
     * Clear Binder proxy
     */
    void NativeLedClient::exit()
    {
         ALOGV("[%s]: enter\n", __FUNCTION__);
         
         BpCustNativeService.clear();
         BpCustNativeService = NULL;       
                        
    }
    
    /**
     * Send init request to binder driver
     */
    int NativeLedClient::init_module()  
    {   
        ALOGV("[%s]: enter\n", __FUNCTION__);
        Parcel data, reply;
        int ret;
          
        data.writeInt32(getpid());    
        ALOGE("[%s]: NativeLedClient::init_module remote()->transact()\n", __FUNCTION__);  
        BpCustNativeService->transact(INIT_NATIVE_LED_MODULE, data, &reply);  
        ret = reply.readInt32();  
        ALOGV("[%s]: ret = %d\n", __FUNCTION__, ret);
        return ret;  
    }  
     
    /**
     * Get binder proxy via system service
     */
    const sp<IBinder> NativeLedClient::getCustNativeService()  
    {  
        ALOGV("[%s]: enter\n",__FUNCTION__);
        if(BpCustNativeService == NULL) {
            sp<IServiceManager> sm = defaultServiceManager();  
            sp<IBinder> binder = sm->getService(String16("native.led.service"));  
            ALOGV("[%s]: defaultServiceManager %p\n",__FUNCTION__, sm.get());  
            if (binder == 0) {  
                ALOGW("getHapticService not published, waiting...");  
            return NULL;  
            }
            ALOGV("[%s]: binder %p\n",__FUNCTION__, binder.get()); 
            BpCustNativeService = binder;
                    
        }
        ALOGV("[%s]: BpCustNativeService %p\n",__FUNCTION__, BpCustNativeService.get()); 
        return BpCustNativeService;  
    }
      
}; //namespace  
 