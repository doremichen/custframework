#
# libNativeLedService
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional


# our own branch needs these headers
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../../../hardware/modules/include/

LOCAL_PRELINK_MODULE := false

LOCAL_LDLIBS := -llog
LOCAL_SHARED_LIBRARIES := \
    liblog \
    libcutils \
    libutils \
    libbinder \
    libhardware
    
LOCAL_SRC_FILES := \
    NativeLedService.cpp \
    NativeLedClient.cpp
                   
LOCAL_MODULE := libNativeLedService
include $(BUILD_SHARED_LIBRARY)

include $(call first-makefiles-under,$(LOCAL_PATH))