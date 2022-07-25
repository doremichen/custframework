LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	adamservice.cpp

LOCAL_LDLIBS := -llog -ldl

LOCAL_SHARED_LIBRARIES := \
            libutils \
            libcutils \
            libmedia \
            libbinder

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= adamservice

include $(BUILD_EXECUTABLE)
