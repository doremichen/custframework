#define LOG_TAG "custframework.hardware.hellohal@1.0-service"

#include <custframework/hardware/hellohal/1.0/IHelloHal.h>

#include <hidl/LegacySupport.h>
#include "HelloHal.h"

using android::sp;

using android::status_t;
using android::OK;


// Generated HIDL files
using custframework::hardware::hellohal::V1_0::IHelloHal;
using custframework::hardware::hellohal::V1_0::implementation::HelloHal;

using android::hardware::defaultPassthroughServiceImplementation;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

int main() {
#if 0
    return defaultPassthroughServiceImplementation<IFoo>();
#else
    android::sp<IHelloHal> service = new HelloHal();
    configureRpcThreadpool(1, true);
    
    status_t status = service->registerAsService();
    
    if (status == OK) {
        ALOGI("Hellohal HAL Ready.");
        joinRpcThreadpool();
    }

    ALOGE("Cannot register foo HAL service");
    return 1;
    
#endif
}
