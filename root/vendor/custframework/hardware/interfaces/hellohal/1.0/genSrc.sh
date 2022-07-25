#! /bin/bash
PACKAGE=custFramework.hardware.hellohal@1.0
LOC=$ANDROID_BUILD_TOP/vendor/custFramework/hardware/interfaces/hellohal/1.0/default/

echo "generate impl and server templeate source code."
hidl-gen -o $LOC -Lc++-impl -rcustFramework.hardware:vendor/custFramework/hardware/interfaces \
-randroid.hidl:system/libhidl/transport $PACKAGE

hidl-gen -o $LOC -Landroidbp-impl -rcustFramework.hardware:vendor/custFramework/hardware/interfaces \
-randroid.hidl:system/libhidl/transport $PACKAGE