/*
 * File: ILed.aidl
 * Brief: binder interface
 *
 * Author: 
 * Date:
 */
 
package com.med.hpframework.hardware.led;

import com.med.hpframework.hardware.led.ILedCallback;

/**
 * {@hide}
 */
interface ILed
{
   int foo();
   
   void registerCallBack(ILedCallback callback);
   void unregisterCallBack(ILedCallback callback);
}
