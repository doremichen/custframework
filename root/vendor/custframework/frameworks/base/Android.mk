#
# Cust framework exercise
#

# the library
# =============================================================
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
            $(call all-subdir-java-files) \
            $(call all-Iaidl-files-under, core)
            
# FRAMEWORKS_BASE_JAVA_SRC_DIRS comes from build/core/pathmap.mk
LOCAL_AIDL_INCLUDES += $(FRAMEWORKS_BASE_JAVA_SRC_DIRS)
LOCAL_AIDL_INCLUDES += $(LOCAL_PATH)/core/java

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE := custframework


include $(BUILD_JAVA_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))

