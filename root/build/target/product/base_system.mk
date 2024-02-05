

# Base modules and settings for the system partition.
###################################################################################
# Sample
PRODUCT_PACKAGES += \
    custhal.default \
    com.custframework.server.xml \
    custframework.hardware.hellohal@1.0 \
    custframework.hardware.hellohal@1.0-service \
    libAIDLService \
    custNativeServer \
    libcust_jni \
    libaidlservice_jni \
    custframework \
    TestCustFramework

PRODUCT_PACKAGES += \
    custrun.sh \
    custToolkits \
    custmonitord

PRODUCT_PACKAGES += \
    cust-jdt-med
    
PRODUCT_PACKAGES += \
    native_service_test
