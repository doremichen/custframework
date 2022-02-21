#
#
# Copy files to device
#
#


LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := run.sh
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := EXECUTABLES


LOCAL_SRC_FILES := $(LOCAL_MODULE)
include $(BUILD_PREBUILT)


#PRODUCT_COPY_FILES += \
#        $(LOCAL_PATH)/run.sh:system/bin/run.sh
       


