#
# Cust framework exercise
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)


LOCAL_MODULE_TAGS := optional

# our own branch needs these headers
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../include/

LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_SHARED_LIBRARIES := liblog libcutils
LOCAL_SRC_FILES := endoscopehal.c
LOCAL_MODULE:= endoscopehal.default
include $(BUILD_SHARED_LIBRARY)