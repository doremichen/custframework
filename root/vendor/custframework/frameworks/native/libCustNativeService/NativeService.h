﻿/**
 * File: NativeService.h
 * Breif: Declare NativeService class extedn BBinder
 *
 * Name: 
 * Date: 
 */

#ifndef CUST_NATIVE_SERVICE_H  
#define CUST_NATIVE_SERVICE_H
  
#include <utils/threads.h>  
#include <utils/RefBase.h>  
#include <binder/IInterface.h>  
#include <binder/BpBinder.h>  
#include <binder/Parcel.h>  
  
namespace android {  
      
class NativeService : public BBinder  
{  
public:
    
    
    enum {
      INIT_NATIVE_MODULE = 1,  
      TURN_ON_EFFECT,
      TURN_OFF_EFFECT
    };  
      
    static int instantiate();  
    NativeService();  
    virtual ~NativeService();  
    virtual status_t onTransact(uint32_t, const Parcel&, Parcel*, uint32_t); 
    
    int module_init();
    int module_on();
    int module_off();
};

  
}; //namespace  
#endif  