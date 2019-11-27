# Cust framework
1. Put this code in the android root/vendor
2. Modify root\build\target\product\common.mk\cmomonlmk as following:
####  Add by adam for cust frameowrk exercise
PRODUCT_PACKAGES += custframework \
                    com.custframework.server.xml \
                    CustManager \
                    libcust_jni \
                    custhal.default \
                    TestCustFramework
3. Add as the fllowing in root\system\core\rootdir\init.rc
# Add by adam for cust frameowrk exercise
#CustManager service (start)
#
service CustManager /system/bin/CustManager
    user root
    group root
#
#CustManager service (end)
#

