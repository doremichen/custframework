#
# This makefile is for nativeservice_test
#
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    nativeservice_test.cpp

LOCAL_C_INCLUDES := \
    vendor/custframework/frameworks/native/libCustNativeService \
    external/gtest/include

LOCAL_STATIC_LIBRARIES := \
    libgtest \
    libgtest_main \

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libutils \
    libCustNativeService \
    libutils \
    libbase

LOCAL_MODULE:= native_service_test

LOCAL_MODULE_TAGS := tests

# LOCAL_MODULE_PATH := $(TARGET_OUT_DATA)/nativetest
LOCAL_MULTILIB := both
LOCAL_MODULE_STEM_32 := native_service_test
LOCAL_MODULE_STEM_64 := native_service_test64

include $(BUILD_EXECUTABLE)
