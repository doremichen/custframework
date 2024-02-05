LOCAL_PATH := $(call my-dir)
  
include $(CLEAR_VARS)

LOCAL_SHARED_LIBRARIES := libcutils \
                          libutils \
                          libbinder \
                          liblog \
                          libcutils

LOCAL_MODULE :=  libAIDLService
LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc
LOCAL_AIDL_INCLUDES := $(LOCAL_PATH)/aidl
LOCAL_SRC_FILES := $(call all-cpp-files-under, src)
LOCAL_SRC_FILES += $(call all-subdir-Iaidl-files, aidl)

include $(BUILD_SHARED_LIBRARY)