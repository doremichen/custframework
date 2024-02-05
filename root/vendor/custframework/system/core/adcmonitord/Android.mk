#
# adc monitor service
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


LOCAL_INIT_RC := custmonitord.rc        #android 7.0
LOCAL_SRC_FILES := custmonitord.cpp
LOCAL_MODULE:= custmonitord
include $(BUILD_EXECUTABLE)