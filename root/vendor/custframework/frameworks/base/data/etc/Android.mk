#
# Cust framework exercise
#

LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := com.custframework.server.xml

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE_CLASS := ETC

# This will install the file in /system/etc/permissions
#
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/permissions

LOCAL_SRC_FILES := $(LOCAL_MODULE)

include $(BUILD_PREBUILT)
