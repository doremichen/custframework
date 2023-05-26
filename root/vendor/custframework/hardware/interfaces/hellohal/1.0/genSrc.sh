#! /bin/bash
PACKAGE=altek.medical.hardware.hellohal@1.0
LOC=$ANDROID_BUILD_TOP/vendor/medical/hardware/interfaces/hellohal/1.0/default/

echo "generate impl and server templeate source code."
hidl-gen -o $LOC -Lc++-impl -raltek.medical.hardware:vendor/medical/hardware/interfaces \
-randroid.hidl:system/libhidl/transport $PACKAGE

hidl-gen -o $LOC -Landroidbp-impl -raltek.medical.hardware:vendor/medical/hardware/interfaces \
-randroid.hidl:system/libhidl/transport $PACKAGE