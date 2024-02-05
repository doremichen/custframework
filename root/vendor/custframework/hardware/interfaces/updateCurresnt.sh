#!/bin/bash
VENDOR_HARDWARE_PATH=vendor/custframework/hardware/interfaces
VENDOR_LABEL=custframework.hardware

echo "update hash current.txt"
hidl-gen -L hash -r ${VENDOR_LABEL}:${VENDOR_HARDWARE_PATH} -r android.hardware:hardware/interfaces -r android.hidl:system/libhidl/transport ${VENDOR_LABEL}.hellohal@1.0 >> $ANDROID_BUILD_TOP/${VENDOR_HARDWARE_PATH}/current.txt