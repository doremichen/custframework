/*
 * File: IAIDLJavaSeviceCallback.aidl
 * Brief: CallBack binder interface
 *
 * Author: 
 * Date:
 */
package com.cust.framework.hardware.aidlSevice;


/**
 * {@hide}
 */
oneway interface IAIDLJavaServiceCallback
{
    void onState(String msg);
}
