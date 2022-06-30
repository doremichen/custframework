// FIXME: your file license if you have one

#pragma once

#include <device/cust/hardware/hellohidl/1.0/IHelloHidl.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace device::cust::hardware::hellohidl::implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct HelloHidl : public V1_0::IHelloHidl {
    // Methods from ::device::cust::hardware::hellohidl::V1_0::IHelloHidl follow.
    Return<void> foo(uint8_t value) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IHelloHidl* HIDL_FETCH_IHelloHidl(const char* name);

}  // namespace device::cust::hardware::hellohidl::implementation
