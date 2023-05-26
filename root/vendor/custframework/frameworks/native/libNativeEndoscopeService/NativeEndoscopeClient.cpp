/**
 * File: NativeEndoscopeClient.cpp
 * Breif: Implement Client class
 *
 * Name: 
 * Date: 
 */

#define LOG_NDEBUG 0  
#define LOG_TAG "NativeEndoscopeClient"
#include <cutils/log.h>

#include "CommonEndoscopeUtils.h"
#include "NativeEndoscopeClient.h"  
  
namespace android  
{  
    sp<IBinder> NativeEndoscopeClient::BpNativeEndoscopeService = NULL;
    
    NativeEndoscopeClient::NativeEndoscopeClient()
    {
        ALOGV("[%s]: enter\n", __FUNCTION__);
        getNativeEndoscopeService();
    }
    
    NativeEndoscopeClient::~NativeEndoscopeClient()
    {
       ALOGV("[%s]: enter\n", __FUNCTION__);
       
       exit();     
            
    }
    
    /**
     * Clear Binder proxy
     */
    void NativeEndoscopeClient::exit()
    {
         ALOGV("[%s]: enter\n", __FUNCTION__);
         
         BpNativeEndoscopeService.clear();
         BpNativeEndoscopeService = NULL;       
                        
    }
    
    /**
     * Send get scope id request to binder driver
     */
    int NativeEndoscopeClient::getScopeId()  
    {   
        ALOGV("[%s]: enter\n", __FUNCTION__);
        Parcel data, reply;
        int ret;
          
        data.writeInt32(getpid());    
        ALOGE("[%s]: NativeEndoscopeClient::getScopeId remote()->transact()\n", __FUNCTION__);  
        BpNativeEndoscopeService->transact(GET_SCOPE_ID, data, &reply);  
        ret = reply.readInt32();  
        ALOGV("[%s]: ret = %d\n", __FUNCTION__, ret);
        return ret;  
    }  
    
    /**
     * Get binder proxy via system service
     */
    const sp<IBinder> NativeEndoscopeClient::getNativeEndoscopeService()  
    {  
        ALOGV("[%s]: enter\n",__FUNCTION__);
        if(BpNativeEndoscopeService == NULL) {
            sp<IServiceManager> sm = defaultServiceManager();  
            sp<IBinder> binder = sm->getService(String16("native.endoscope.service"));  
            ALOGV("[%s]: defaultServiceManager %p\n",__FUNCTION__, sm.get());  
            if (binder == 0) {  
                ALOGW("Service not published, waiting...");  
            return NULL;  
            }
            ALOGV("[%s]: binder %p\n",__FUNCTION__, binder.get()); 
            BpNativeEndoscopeService = binder;
                    
        }
        ALOGV("[%s]: BpNativeEndoscopeService %p\n",__FUNCTION__, BpNativeEndoscopeService.get()); 
        return BpNativeEndoscopeService;  
    }
      
}; //namespace  
 