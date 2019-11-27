/**
 * File: CustClient.h
 * Breif: Declare Client class
 *
 * Name: Adam chen
 * Date: 
 */

#ifndef CUST_CLIENT__H  
#define CUST_CLIENT__H  

#include <utils/RefBase.h>

#include <binder/IServiceManager.h>  
#include <binder/IPCThreadState.h>  
  
namespace android  
{  

class CustClient : public RefBase
{  
public:
     enum {
        INIT_MODULE = 1,
        LCD_TURN_ON,
        LCD_TURN_OFF          
     };   
    
    
     virtual ~CustClient();
     CustClient();
     
     static  sp<IBinder> BpCustNativeService;
     static  const sp<IBinder> getCustNativeService();
     
     int init_module();
     int turn_on_lcd();
     int turn_off_lcd();
     
     
     void exit();
                 
};  

}; //namespace   
#endif // CUST_CLIENT__H  