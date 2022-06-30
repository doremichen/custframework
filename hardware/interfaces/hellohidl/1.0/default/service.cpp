/*
 * hello hidl service
 */

#define LOG_TAG "device.cust.hardware.hellohidl@1.0-service"

#include <device/cust/hardware/hellohidl/1.0/IHelloHidl.h>
#include <hidl/HidlTransportSupport.h>
#include <log/log.h>
#include "HelloHidl.h"

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using device::cust::hardware::hellohidl::V1_0::IHelloHidl;
using device::cust::hardware::hellohidl::implementation::HelloHidl;

int main(int /* argc */, char** /* argv */) {
    configureRpcThreadpool(1, true);

    android::sp<IHelloHidl> sensors = new HelloHidl();
    if (sensors->registerAsService() != ::android::OK) {
        ALOGE("Failed to register Sensors HAL instance");
        return -1;
    }

    joinRpcThreadpool();
    return 1;  // joinRpcThreadpool shouldn't exit
}
