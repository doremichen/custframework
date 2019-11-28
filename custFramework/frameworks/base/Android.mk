#
# Cust framework exercise
#

# the library
# =============================================================
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
            $(call all-subdir-java-files)

LOCAL_MODULE_TAGS := optinal

LOCAL_MODULE := custframework

#
#LOCAL_MODULE_CLASS := JAVA_LIBRARIES
#LOCAL_JAVA_LIBRARIES := android.policy

# AIDL
LOCAL_SRC_FILES += \
        core/java/custdroid/hardware/ISpecialEffectService.aidl

include $(BUILD_JAVA_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))

