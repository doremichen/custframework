#
# Cust framework exercise
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_ROOT := custFramework

# our own branch needs these headers
LOCAL_C_INCLUDES += \
    $(TOP)/vendor/$(LOCAL_ROOT)/hardware/modules/include/

LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_LDLIBS := -llog
LOCAL_SHARED_LIBRARIES := \
    liblog \
    libcutils \
    libutils \
    libbinder \
    libhardware
    
LOCAL_SRC_FILES := \
    NativeService.cpp \
    CustClient.cpp
                   
LOCAL_MODULE := libCustNativeService
include $(BUILD_SHARED_LIBRARY)