#
# This makefile is for hapticservice_test
#
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	NativeService_test.cpp

LOCAL_C_INCLUDES := \
    vendor/custframework/frameworks/native/libCustNativeService \
    bionic \
    bionic/libstdc++/include \
    external/gtest/include \
    external/stlport/stlport \

LOCAL_STATIC_LIBRARIES := \
	libgtest \
	libgtest_main \

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	libCustNativeService \
	libutils \
    libstlport

LOCAL_MODULE:= nativeService_test

LOCAL_MODULE_TAGS := tests

# LOCAL_MODULE_PATH := $(TARGET_OUT_DATA)/nativetest

include $(BUILD_EXECUTABLE)
