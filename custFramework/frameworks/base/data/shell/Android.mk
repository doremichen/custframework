#
#
# Copy files to device
#
#


LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

PRODUCT_COPY_FILES += \
        $(LOCAL_PATH)/run.sh:system/bin/run.sh
       


