# Cust framework
## This exercise project contains java framework and native framework design. The design item is as the following
 + a. binder  
 + b. jni invoking process (forword/backword)  
 + c. hal module  
## Build sop
1. Get ASOP  
2. Put this code in the android root\vendor  
3. Add as the followng in root\build\target\product\base_system.mk  
PRODUCT_PACKAGES += custframework \\  
                    com.custframework.server.xml \\  
                    custsvr \\  
                    libcust_jni \\  
                    custhal.default \\  
                    TestCustFramework
4. Add SELinux policy as the following example:  
      a. create te file in the specified diractory  
         device\sample\sepolicy\generic\vendor\common\custsvr.te  
      b. Add line in file_contexts file as following  
         /(vendor|system|system/vendor)/bin/custsvr      u:object_r:hpservice_exec:s0  
~4. Add as the fllowing in root\system\core\rootdir\init.rc~  
~service CustManager /system/bin/CustManager~  
~user root~ 
~group root~ 

## Note
In Android 5.0 and later, SELinux is fully enforced, building on the permissive release of Android 4.3 and the partial enforcement of Android 4.4. With this change, Android shifted from enforcement on a limited set of crucial domains (installd, netd, vold and zygote) to everything (more than 60 domains).
