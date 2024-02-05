/*
 * File: IAIDLJavaSevice.aidl
 * Brief: binder interface
 *
 * Author: 
 * Date:
 */
package com.cust.framework.hardware.aidlSevice;

import com.cust.framework.hardware.aidlSevice.IAIDLJavaServiceCallback;

/**
 * {@hide}
 */
interface IAIDLJavaService
{
   void start();
   void testCallback();
   
   void registerCallBack(IAIDLJavaServiceCallback callback);
   void unregisterCallBack(IAIDLJavaServiceCallback callback);
}
