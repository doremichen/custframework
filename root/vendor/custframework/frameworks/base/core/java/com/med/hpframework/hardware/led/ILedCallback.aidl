/*
 * File: ILedCallback.aidl
 * Brief: CallBack binder interface
 *
 * Author: 
 * Date:
 */
 
package com.med.hpframework.hardware.led;


/**
 * {@hide}
 */
oneway interface ILedCallback
{
    void info(String msg);
}
