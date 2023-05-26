/**
 * ADC button service implement
 */
#define LOG_NDEBUG 0  
#define LOG_TAG "ADCButtonService"
#include <cutils/log.h>

#include "ADCButtonService.h"

using namespace android;
using namespace com::med::adc::button::service;

sp<IADCButtonCallback> ADCButtonService::sCallback = nullptr;

void 
onButtonEvent(const char* event)
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    ADCButtonService::sCallback -> onButton(String16(event));
    ALOGV("[%s] exit\n", __FUNCTION__);
}


namespace com {
namespace med {
namespace adc {
namespace button {
namespace service {

    ::android::binder::Status  ADCButtonService::init() {
       ALOGV("[%s] enter\n", __FUNCTION__);
       start_detect_iio_event(onButtonEvent);
       ALOGV("[%s] exit\n", __FUNCTION__);
       return ::android::binder::Status::ok();
   }

   ADCButtonService:: ADCButtonService() {
   }
   ADCButtonService:: ~ADCButtonService() {
   }

 ::android::binder::Status ADCButtonService::setCallback(const ::android::sp<IADCButtonCallback> &cb) {
       ALOGV("[%s] enter\n", __FUNCTION__);
       ALOGV("cb: [%p] \n", cb.get());
       ADCButtonService::sCallback = cb;
       ALOGV("sCallback: [%p] \n", ADCButtonService::sCallback.get());
       ALOGV("[%s] exit\n", __FUNCTION__);
       return ::android::binder::Status::ok();
   }

   int ADCButtonService::instantiate()  
    {  
        ALOGV("ADCButtonService instantiate"); 
        
        //  
        int r = defaultServiceManager()->addService(String16(SERVICE_NAME), 
                new ADCButtonService());  
        ALOGV("ADCButtonService r = %d/n", r);
 
        return r;  
    }  

} // namespace service
} // namespace button
} // namespace adc
} // namespace med
} // namespace com