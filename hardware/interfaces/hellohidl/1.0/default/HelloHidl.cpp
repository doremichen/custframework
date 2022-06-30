// FIXME: your file license if you have one

#include <stdio.h>

#include "HelloHidl.h"

namespace device::cust::hardware::hellohidl::implementation {

// Methods from ::device::cust::hardware::hellohidl::V1_0::IHelloHidl follow.
Return<void> HelloHidl::foo(uint8_t value) {
    printf("[%s]  value = %d\n", __FUNCTION__, value);
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IHelloHidl* HIDL_FETCH_IHelloHidl(const char* /* name */) {
    //return new HelloHidl();
//}
//
}  // namespace device::cust::hardware::hellohidl::implementation
