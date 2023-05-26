// FIXME: your file license if you have one

#include <android-base/logging.h>

#include "HelloHal.h"

namespace altek {
namespace medical {
namespace hardware {
namespace hellohal {
namespace V1_0 {
namespace implementation {
    
// Methods from ::altek::medical::hardware::hellohal::V1_0::IHelloHal follow.
Return<void> HelloHal::foo(const hidl_string& name, foo_cb _hidl_cb) {
    LOG(INFO) <<__FUNCTION__<< "function is called!!!";
    LOG(INFO) << "name: " << name.c_str() << "";
    _hidl_cb(name, HelloTest::INVALID);
    return Void();
}

Return<void> HelloHal::bar(::altek::medical::hardware::hellohal::V1_0::HelloTest value) {
    LOG(INFO) <<__FUNCTION__<< "function is called!!!";
    uint8_t v = static_cast<uint8_t>(value);
    LOG(INFO) << "value: " << v << "";
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IHelloHal* HIDL_FETCH_IHelloHal(const char* /* name */) {
    //return new HelloHal();
//}
//
}  // namespace implementation
}  // namespace V1_0
}  // namespace hellohal
}  // namespace hardware
}  // namespace medical
}  // namespace altek
