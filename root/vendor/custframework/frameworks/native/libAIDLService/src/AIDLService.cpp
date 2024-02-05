/**
 * cust aidl service implement
 */
#define LOG_NDEBUG 0  
#define LOG_TAG "AIDLService"
#include <cutils/log.h>

#include "AIDLService.h"

using namespace android;
using namespace com::cust::service;

sp<IAIDLServiceCallback> AIDLService::sCallback = nullptr;

namespace com {
namespace cust {
namespace service {

    ::android::binder::Status  AIDLService::init() {
       ALOGV("[%s] enter\n", __FUNCTION__);
       ALOGV("[%s] exit\n", __FUNCTION__);
       return ::android::binder::Status::ok();
   }
   
   ::android::binder::Status  AIDLService::testCallback() {
       ALOGV("[%s] enter\n", __FUNCTION__);
       const char* event = "Hello!!! I am from native cust aidl service";
       if (AIDLService::sCallback == nullptr) {
           return ::android::binder::Status::ok();
       }
       
       AIDLService::sCallback -> onCallback(String16(event));
       ALOGV("[%s] exit\n", __FUNCTION__);
       return ::android::binder::Status::ok();
   }

   AIDLService:: AIDLService() {
   }
   AIDLService:: ~AIDLService() {
   }

 ::android::binder::Status AIDLService::setCallback(const ::android::sp<IAIDLServiceCallback> &cb) {
       ALOGV("[%s] enter\n", __FUNCTION__);
       ALOGV("cb: [%p] \n", cb.get());
       AIDLService::sCallback = cb;
       ALOGV("sCallback: [%p] \n", AIDLService::sCallback.get());
       ALOGV("[%s] exit\n", __FUNCTION__);
       return ::android::binder::Status::ok();
   }

   int AIDLService::instantiate()  
    {  
        ALOGV("AIDLService instantiate"); 
        
        //  
        int r = defaultServiceManager()->addService(String16(SERVICE_NAME), 
                new AIDLService());  
        ALOGV("AIDLService r = %d/n", r);
 
        return r;  
    }  

} // namespace service
} // service
} // namespace com