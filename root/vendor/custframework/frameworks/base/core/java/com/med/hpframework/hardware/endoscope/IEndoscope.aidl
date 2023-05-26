/*
 * File: IEndoscope.aidl
 * Brief: binder interface
 *
 * Author: 
 * Date:
 */
 
package com.med.hpframework.hardware.endoscope;

import com.med.hpframework.hardware.endoscope.IEndoscopeCallback;

/**
 * {@hide}
 */
interface IEndoscope
{
   int getEndoScopeId();
   
   void registerCallBack(IEndoscopeCallback callback);
   void unregisterCallBack(IEndoscopeCallback callback);
}
