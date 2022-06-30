#!/bin/bash


hidl-gen -L hash -r device.cust.hardware:device/custframework/hardware/interfaces -r android.hardware:hardware/interfaces -r android.hidl:system/libhidl/transport device.cust.hardware.hellohidl@1.0 >> $ANDROID_BUILD_TOP/device/custframework/hardware/interfaces/current.txt
