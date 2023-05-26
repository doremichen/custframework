#
# Test service app
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_USE_AAPT2 := true

LOCAL_MODULE_TAGS := optional

LOCAL_PRIVATE_PLATFORM_APIS := true
LOCAL_CERTIFICATE := platform
LOCAL_PRODUCT_MODULE := true
LOCAL_PRIVILEGED_MODULE := true

LOCAL_SRC_FILES := $(call all-java-files-under, src)
LOCAL_RESOURCE_DIR := $(addprefix $(LOCAL_PATH)/, res)

LOCAL_PACKAGE_NAME := TestServiceApp

LOCAL_REQUIRED_MODULES := privapp_whitelist_com.med.app.test.service

# Also link against our own custom library.
LOCAL_JAVA_LIBRARIES := hpframework junit android.test.mock.stubs
LOCAL_STATIC_JAVA_LIBRARIES := \
    android.test.base-minus-junit \
    android.test.runner-minus-junit

include $(BUILD_PACKAGE)
