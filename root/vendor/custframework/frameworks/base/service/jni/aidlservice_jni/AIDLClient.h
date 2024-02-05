/**
 * File: AIDLClient.h
 * Breif: Declare Client class
 *
 * Name: 
 * Date: 
 */

#ifndef NATIVE_AIDL_CLIENT_H  
#define NATIVE_AIDL_CLIENT_H  

#include <utils/RefBase.h>

#include "AIDLServiceCallback.h"

using namespace std;
using namespace com::cust::service;
  
namespace android  
{  
class AIDLClient : public RefBase
{  
public:

     virtual ~AIDLClient();
     AIDLClient();
     static sp<IAIDLService> BpAIDLService;
     static  const sp<IAIDLService> getAIDLService();
     
     int init_module();
     void setAIDLServiceCallback(const sp<IAIDLServiceCallback>& callback);
     
     void exit();
                 
};  

}; //namespace   
#endif // NATIVE_AIDL_CLIENT_H  