#
#
#


LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

# our own branch needs these headers
LOCAL_C_INCLUDES += \
    $(TOP)/vendor/Custdroid/frameworks/native/libCustNativeService


LOCAL_PRELINK_MODULE := false

LOCAL_LDLIBS := -llog
LOCAL_SHARED_LIBRARIES := \
    liblog \
    libcutils \
    libutils \
    libbinder \
    libCustNativeService
    
LOCAL_SRC_FILES := CustManager.cpp
LOCAL_MODULE := CustManager
include $(BUILD_EXECUTABLE)
