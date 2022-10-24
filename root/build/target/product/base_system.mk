

# Base modules and settings for the system partition.
PRODUCT_PACKAGES += \
    custhal.default \
    com.custframework.server.xml \
    libCustNativeService \
    libcust_jni \
    custsvr \
    custframework \
    TestCustFramework \
    
# for native test case
PRODUCT_PACKAGES += \
    native_service_test
