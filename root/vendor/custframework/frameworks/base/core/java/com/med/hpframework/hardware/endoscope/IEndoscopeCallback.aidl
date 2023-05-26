/*
 * File: IEndoscopeCallback.aidl
 * Brief: CallBack binder interface
 *
 * Author: 
 * Date:
 */
 
package com.med.hpframework.hardware.endoscope;


/**
 * {@hide}
 */
oneway interface IEndoscopeCallback
{
    void info(String msg);
}
