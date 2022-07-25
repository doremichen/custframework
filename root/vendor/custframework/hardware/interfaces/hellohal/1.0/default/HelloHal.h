// FIXME: your file license if you have one

#pragma once

#include <custFramework/hardware/hellohal/1.0/IHelloHal.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace custFramework {
namespace hardware {
namespace hellohal {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct HelloHal : public IHelloHal {
    // Methods from ::custFramework::hardware::hellohal::V1_0::IHelloHal follow.
    Return<void> foo(const hidl_string& name, foo_cb _hidl_cb) override;
    Return<void> bar(::custFramework::hardware::hellohal::V1_0::HelloTest value) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IHelloHal* HIDL_FETCH_IHelloHal(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace hellohal
}  // namespace hardware
}  // namespace custFramework
