// File: CustClient.cpp
#define LOG_NDEBUG 0  
#define LOG_TAG "CustClient"
#include <cutils/log.h>

#include "CustClient.h"  
  
namespace android  
{  
    sp<IBinder> CustClient::BpCustNativeService = NULL;
    
    CustClient::CustClient()
    {
        ALOGV("[%s]: enter\n", __FUNCTION__);
        getCustNativeService();
    }
    
    CustClient::~CustClient()
    {
       ALOGV("[%s]: enter\n", __FUNCTION__);
       
       exit();     
            
    }
    
    void CustClient::exit()
    {
         ALOGV("[%s]: enter\n", __FUNCTION__);
         
         BpCustNativeService.clear();
         BpCustNativeService = NULL;       
                        
    }
    
    int CustClient::init_module()  
    {   
        ALOGV("[%s]: enter\n", __FUNCTION__);
        Parcel data, reply;  
        int ret;  
          
        data.writeInt32(getpid());    
        ALOGE("[%s]: CustClient::init_module remote()->transact()\n", __FUNCTION__);  
        BpCustNativeService->transact(INIT_MODULE, data, &reply);  
        ret = reply.readInt32();  
        ALOGV("[%s]: ret = %d\n", __FUNCTION__, ret);      
        return ret;  
    }  
    
    
    int CustClient::turn_on_lcd()  
    {   
        ALOGV("[%s]: enter\n", __FUNCTION__);
        Parcel data, reply;  
        int ret;  
          
        data.writeInt32(getpid());    
        ALOGE("[%s]: CustClient::turn_on_lcd remote()->transact()\n", __FUNCTION__);  
        BpCustNativeService->transact(LCD_TURN_ON, data, &reply);  
        ret = reply.readInt32();  
        ALOGV("[%s]: ret = %d\n", __FUNCTION__, ret);      
        return ret;  
    }  
    
    int CustClient::turn_off_lcd()  
    {   
        ALOGV("[%s]: enter\n", __FUNCTION__);
        Parcel data, reply;  
        int ret;  
          
        data.writeInt32(getpid());   
        ALOGE("[%s]: CustClient::turn_off_lcd remote()->transact()\n", __FUNCTION__);  
        BpCustNativeService->transact(LCD_TURN_OFF, data, &reply);  
        ret = reply.readInt32();  
        ALOGV("[%s]: ret = %d\n", __FUNCTION__, ret);      
        return ret;  
    }  
     
  
    const sp<IBinder> CustClient::getCustNativeService()  
    {  
        ALOGV("[%s]: enter\n",__FUNCTION__);
        if(BpCustNativeService == NULL) {
            sp<IServiceManager> sm = defaultServiceManager();  
            sp<IBinder> binder = sm->getService(String16("cust.native.service"));  
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
 