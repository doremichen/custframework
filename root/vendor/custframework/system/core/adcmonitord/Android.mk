#
# ADS7914 service
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)


LOCAL_MODULE_TAGS := optional

LOCAL_PRELINK_MODULE := false

LOCAL_LDLIBS := -llog -ldl

LOCAL_CFLAGS := -Wall -Wextra -Werror

LOCAL_CFLAGS += -Wno-unused-parameter

#LOCAL_SHARED_LIBRARIES := libbase

LOCAL_SHARED_LIBRARIES := \
    liblog \
    libcutils \
    libutils \


LOCAL_INIT_RC := adcmonitord.rc        #android 7.0
LOCAL_SRC_FILES := adcmonitord.cpp
LOCAL_MODULE:= adcmonitord
include $(BUILD_EXECUTABLE)