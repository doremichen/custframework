/**
 * ADC button service header
 */
#ifndef _ADC_BUTTON_SERVICE_H
#define _ADC_BUTTON_SERVICE_H

#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <thread>
#include <chrono>
#include <atomic>
#include<condition_variable>

#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/IBinder.h>
#include <binder/Binder.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>

#include <com/med/adc/button/service/BnADCButtonService.h>
#include <com/med/adc/button/service/BnADCButtonCallback.h>

using namespace android;
using namespace std;
using namespace com::med::adc::button::service;


#define SERVICE_NAME "native.adc.button.service"

namespace com {
namespace med {
namespace adc {
namespace button {
namespace service {
    class ADCButtonService : public BnADCButtonService {
        public:
        static int instantiate();
        ::android::binder::Status setCallback(const ::android::sp<IADCButtonCallback>& cb);
        ::android::binder::Status init();
        static sp<IADCButtonCallback> sCallback;
    private:
        ADCButtonService();
        ~ADCButtonService();
    };
} // namespace service
} // namespace button
} // namespace adc
} // namespace med
} // namespace com

typedef void (*BUTTON_EVENT)(const char* event);

int start_detect_iio_event(BUTTON_EVENT eventFunc);


#endif /* _ADC_BUTTON_SERVICE_H */