# Cust framework
1. Put this code in the android root\vendor
2. Add as the followng in root\build\target\product\common.mk\cmomonlmk  
PRODUCT_PACKAGES += custframework \\  
                    com.custframework.server.xml \\  
                    CustManager \\  
                    libcust_jni \\  
                    custhal.default \\  
                    TestCustFramework  
3. Add as the fllowing in root\system\core\rootdir\init.rc  
service CustManager /system/bin/CustManager  
    user root  
    group root  

