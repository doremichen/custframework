#
# Cust framework exercise
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ROOT := custFramework

LOCAL_MODULE_TAGS := optional

# our own branch needs these headers
LOCAL_C_INCLUDES += \
    $(TOP)/vendor/$(LOCAL_ROOT)/hardware/modules/include/

LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_SHARED_LIBRARIES := liblog libcutils
LOCAL_SRC_FILES := custhal.c
LOCAL_MODULE:= custhal.default
include $(BUILD_SHARED_LIBRARY)
