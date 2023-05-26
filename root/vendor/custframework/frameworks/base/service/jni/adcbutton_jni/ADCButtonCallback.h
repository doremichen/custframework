/**
 * ADC Button Callback
 */
#ifndef _ADC_BUTTON_CALLBACK_H
#define _ADC_BUTTON_CALLBACK_H

#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

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

namespace com {
namespace med {
namespace adc {
namespace button {
namespace service {
    class ADCButtonCallback : public BnADCButtonCallback {
    public:
        static sp<ADCButtonCallback> getInstance();
        virtual ::android::binder::Status onButton(const ::android::String16& event);
    private:
           ADCButtonCallback();
           ~ADCButtonCallback();
    };
} // namespace service
} // namespace button
} // namespace adc
} // namespace med
} // namespace com
#endif /* _ADC_BUTTON_CALLBACK_H */
