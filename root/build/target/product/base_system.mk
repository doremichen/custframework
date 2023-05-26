

# Base modules and settings for the system partition.
PRODUCT_PACKAGES += \
    endoscopehal.default \
    custhal.default \
    com.hpframework.server.xml \
    libNativeEndoscopeService \
    libNativeLedService \
    libCustNativeService \
    libADCButtonService \
    libendoscope_jni \
    libled_jni \
    libserialport_jni \
    libadc_button_jni \
    hpservice \
    adcmonitord \
    hpframework \
    TestCustFramework \
    TestServiceApp \
    
    
# for native test case
PRODUCT_PACKAGES += \
    native_service_test

# for medical external lib
PRODUCT_HOST_PACKAGES += \
    jdt \
    usblib \

PRODUCT_PACKAGES += \
    jdt-med \
    usblib-med \
    uvclib-med \

# for medical platform
PRODUCT_PACKAGES += \
    TestServiceApp

# for usblib jni
PRODUCT_PACKAGES += \
    errno-lib

# altek hellword hal
PRODUCT_PACKAGES += \
    altek.medical.hardware.hellohal@1.0-service
