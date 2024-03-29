#
# aidl service jni
#

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

# [optional, user, eng] 
# eng = required
# optinal = no install on target
LOCAL_MODULE_TAGS := optional

# This is the target being built.
LOCAL_MODULE:= libaidlservice_jni

# All of the source files that we will compile.
LOCAL_SRC_FILES := $(call all-cpp-files-under, .)

# All of the shared libraries we link against.
LOCAL_SHARED_LIBRARIES := \
    libbase \
    libandroid_runtime \
    libnativehelper \
    libbinder \
    libcutils \
    libutils \
    libAIDLService
    
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

# No static libraries.
LOCAL_STATIC_LIBRARIES :=

# Also need the JNI headers.
LOCAL_C_INCLUDES += \
    $(JNI_H_INCLUDE)

# No specia compiler flags.
LOCAL_CFLAGS +=

# Don't prelink this library.  For more efficient code, you may want
# to add this library to the prelink map and set this to true.
LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
