# Cust framework
## This exercise project contains java framework and native framework design. The design item is as the following
 + a. binder  
 + b. jni invoking process (forword/backword)  
 + c. hal module  
## Build sop
1. Get ASOP  
2. Put this code in the android root\vendor  
3. Add as the followng in root\build\target\product\common.mk\cmomonlmk  
PRODUCT_PACKAGES += custframework \\  
                    com.custframework.server.xml \\  
                    CustManager \\  
                    libcust_jni \\  
                    custhal.default \\  
                    TestCustFramework  
4. Add as the fllowing in root\system\core\rootdir\init.rc  
service CustManager /system/bin/CustManager  
    user root  
    group root  

