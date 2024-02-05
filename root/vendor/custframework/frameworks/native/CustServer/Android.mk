#
# Cust framework exercise
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

# our own branch needs these headers
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../libCustNativeService \
    $(LOCAL_PATH)/../libAIDLService/inc \


LOCAL_PRELINK_MODULE := false

LOCAL_LDLIBS := -llog
LOCAL_SHARED_LIBRARIES := \
    liblog \
    libcutils \
    libutils \
    libbinder \
    libCustNativeService \
    libAIDLService \


LOCAL_INIT_RC := custserver.rc        #android 7.0


LOCAL_SRC_FILES := CustManager.cpp
LOCAL_MODULE := custNativeServer
include $(BUILD_EXECUTABLE)
