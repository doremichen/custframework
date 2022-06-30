#! /bin/bash
PACKAGE=device.cust.hardware.hellohidl@1.0
LOC=$ANDROID_BUILD_TOP/device/custframework/hardware/interfaces/hellohidl/1.0/default/

hidl-gen -o $LOC -Lc++-impl -rdevice.cust.hardware:device/custframework/hardware/interfaces \
-randroid.hidl:system/libhidl/transport $PACKAGE

hidl-gen -o $LOC -Landroidbp-impl -rdevice.cust.hardware:device/custframework/hardware/interfaces \
-randroid.hidl:system/libhidl/transport $PACKAGE
