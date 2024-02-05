#
# This makefile is for nativeservice_test
#
# local_target_dir := $(TARGET_OUT_DATA)/local/tmp
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    nativeservice_test.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../  \
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


# LOCAL_MODULE_PATH := $(local_target_dir)
LOCAL_MULTILIB := both
LOCAL_MODULE_STEM_32 := native_service_test
LOCAL_MODULE_STEM_64 := native_service_test64

include $(BUILD_EXECUTABLE)
