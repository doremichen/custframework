/**
 * File: NativeEndoscopeService.h
 * Breif: Declare NativeEndoscopeService class extedn BBinder
 *
 * Name: 
 * Date: 
 */

#ifndef NATIVE_ENDOSCOPE_SERVICE_H  
#define NATIVE_ENDOSCOPE_SERVICE_H
  
#include <utils/threads.h>  
#include <utils/RefBase.h>  
#include <binder/IInterface.h>  
#include <binder/BpBinder.h>  
#include <binder/Parcel.h>  
  
namespace android {  
      
class NativeEndoscopeService : public BBinder  
{  
public:
    
    static int instantiate();  
    NativeEndoscopeService();  
    virtual ~NativeEndoscopeService();  
    virtual status_t onTransact(uint32_t, const Parcel&, Parcel*, uint32_t); 
    
    int getScopeId();
};

  
}; //namespace  
#endif  //NATIVE_ENDOSCOPE_SERVICE_H