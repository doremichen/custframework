#
#
# Copy files to device
#
#


LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := custrun.sh
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := EXECUTABLES


LOCAL_SRC_FILES := run.sh
include $(BUILD_PREBUILT)


#PRODUCT_COPY_FILES += \
#        $(LOCAL_PATH)/run.sh:system/bin/run.sh
       


