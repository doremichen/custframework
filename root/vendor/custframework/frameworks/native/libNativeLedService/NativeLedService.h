/**
 * File: NativeLedService.h
 * Breif: Declare NativeLedService class extedn BBinder
 *
 * Name: 
 * Date: 
 */

#ifndef NATIVE_LED_SERVICE  
#define NATIVE_LED_SERVICE
  
#include <utils/threads.h>  
#include <utils/RefBase.h>  
#include <binder/IInterface.h>  
#include <binder/BpBinder.h>  
#include <binder/Parcel.h>  
  
namespace android {  
      
class NativeLedService : public BBinder  
{  
public:

    static int instantiate();  
    NativeLedService();  
    virtual ~NativeLedService();  
    virtual status_t onTransact(uint32_t, const Parcel&, Parcel*, uint32_t); 
    
    int module_init();
};

  
}; //namespace  
#endif  