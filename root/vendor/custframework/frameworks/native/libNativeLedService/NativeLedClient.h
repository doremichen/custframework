/**
 * File: NativeLedClient.h
 * Breif: Declare Client class
 *
 * Name: 
 * Date: 
 */

#ifndef NATIVE_LED_CLIENT_H  
#define NATIVE_LED_CLIENT_H  

#include <utils/RefBase.h>

#include <binder/IServiceManager.h>  
#include <binder/IPCThreadState.h>  
  
namespace android  
{  

class NativeLedClient : public RefBase
{  
public:

     virtual ~NativeLedClient();
     NativeLedClient();
     
     static  sp<IBinder> BpCustNativeService;
     static  const sp<IBinder> getCustNativeService();
     
     int init_module();
     
     void exit();
                 
};  

}; //namespace   
#endif // NATIVE_LED_CLIENT_H  