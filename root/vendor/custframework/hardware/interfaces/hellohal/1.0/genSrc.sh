#! /bin/bash
PACKAGE=custframework.hardware.hellohal@1.0
LOC=$ANDROID_BUILD_TOP/vendor/custframework/hardware/interfaces/hellohal/1.0/default/

echo "generate impl and server templeate source code."
hidl-gen -o $LOC -Lc++-impl -rcustframework.hardware:vendor/custframework/hardware/interfaces \
-randroid.hidl:system/libhidl/transport $PACKAGE

hidl-gen -o $LOC -Landroidbp-impl -rcustframework.hardware:vendor/custframework/hardware/interfaces \
-randroid.hidl:system/libhidl/transport $PACKAGE