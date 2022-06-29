#
# Cust framework exercise
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

ifneq (,$(wildcard frameworks/base))
  LOCAL_PRIVATE_PLATFORM_APIS := true
else
  LOCAL_SDK_VERSION := system_current
  LOCAL_MIN_SDK_VERSION := 26
endif


LOCAL_SRC_FILES := $(call all-java-files-under, src)
#LOCAL_SDK_VERSION := current

LOCAL_PACKAGE_NAME := TestCustFramework

# We need to assign platform key to use ServiceManager.addService.
LOCAL_CERTIFICATE := platform

# Also link against our own custom library.
LOCAL_JAVA_LIBRARIES := custframework

#LOCAL_OVERRIDES_PACKAGES :=

#LOCAL_PROGUARD_FLAG_FILES :=

include $(BUILD_PACKAGE)
