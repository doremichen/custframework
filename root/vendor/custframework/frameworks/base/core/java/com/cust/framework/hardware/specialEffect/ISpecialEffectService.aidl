/*
 * File: ISpecialEffectService.aidl
 * Brief: This is aidl interface
 *
 * Author: 
 * Date: 109/11/27
 */
 
package com.cust.framework.hardware.specialEffect;

import com.cust.framework.hardware.specialEffect.IServiceCallBack;

/**
 * {@hide}
 */
interface ISpecialEffectService
{
   boolean SpecialEffectInit();
   boolean SpecialEffectsOn();
   boolean SpecialEffectsOff();
   void release();
   
   void registerCallBack(IServiceCallBack callback);
   void unregisterCallBack(IServiceCallBack callback);
}
