#
# Cust framework exercise
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_USE_AAPT2 := true

LOCAL_MODULE_TAGS := optional

LOCAL_PRIVATE_PLATFORM_APIS := true

#LOCAL_SDK_VERSION := current
#LOCAL_MIN_SDK_VERSION := 26

LOCAL_SRC_FILES := $(call all-java-files-under, src)
LOCAL_RESOURCE_DIR := $(addprefix $(LOCAL_PATH)/, res)

LOCAL_PACKAGE_NAME := TestCustFramework

# We need to assign platform key to use ServiceManager.addService.
LOCAL_CERTIFICATE := platform

# Also link against our own custom library.
LOCAL_JAVA_LIBRARIES := custframework

include $(BUILD_PACKAGE)
