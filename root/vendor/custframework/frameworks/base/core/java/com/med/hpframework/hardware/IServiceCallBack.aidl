/*
 * File: IServiceCallBack.aidl
 * Brief: This is aidl interface
 *
 * Author: 
 * Date: 109/11/28
 */
 
package com.med.hpframework.hardware;


/**
 * {@hide}
 */
oneway interface IServiceCallBack
{
    void info(String msg);
}
