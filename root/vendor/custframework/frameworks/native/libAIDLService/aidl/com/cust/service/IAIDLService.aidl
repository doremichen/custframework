//
// cust AIDL service
//
package com.cust.service;
  
import com.cust.service.IAIDLServiceCallback;  

interface IAIDLService {
    void init();
    void setCallback(IAIDLServiceCallback cb);
    void testCallback();
}
