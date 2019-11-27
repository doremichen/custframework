#
# Cust framework exercise
#

# This makefile supplies the rules for building a library of JNI code for
# use by our example platform shared library.

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

# [optional, user, eng] 
# eng = required
# optinal = no install on target
LOCAL_MODULE_TAGS := optinal

LOCAL_ROOT := custFramework

# This is the target being built.
LOCAL_MODULE:= libcust_jni

# Target install path.
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)

# All of the source files that we will compile.
LOCAL_SRC_FILES:= \
    com_cust_server_SpecialEffectsService.cpp

# All of the shared libraries we link against.
LOCAL_SHARED_LIBRARIES := \
    libandroid_runtime \
    libnativehelper \
    libcutils \
    libutils \
    libCustNativeService

# No static libraries.
LOCAL_STATIC_LIBRARIES :=

# Also need the JNI headers.
LOCAL_C_INCLUDES += \
    $(JNI_H_INCLUDE) \
    $(TOP)/vendor/$(LOCAL_ROOT)/frameworks/native/libCustNativeService

# No specia compiler flags.
LOCAL_CFLAGS +=

# Don't prelink this library.  For more efficient code, you may want
# to add this library to the prelink map and set this to true.
LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
