/**
 * File: NativeEndoscopeClient.h
 * Breif: Declare Client class
 *
 * Name: 
 * Date: 
 */

#ifndef NATIVE_ENDOSCOPE_CLIENT__H  
#define NATIVE_ENDOSCOPE_CLIENT__H  

#include <utils/RefBase.h>

#include <binder/IServiceManager.h>  
#include <binder/IPCThreadState.h>  
  
namespace android  
{  

class NativeEndoscopeClient : public RefBase
{  
public:
    
     virtual ~NativeEndoscopeClient();
     NativeEndoscopeClient();
     
     static  sp<IBinder> BpNativeEndoscopeService;
     static  const sp<IBinder> getNativeEndoscopeService();
     
     int getScopeId();
     
     void exit();
};  

}; //namespace   
#endif // NATIVE_ENDOSCOPE_CLIENT__H  