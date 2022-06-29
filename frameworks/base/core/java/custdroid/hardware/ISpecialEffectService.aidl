/*
 * File: ISpecialEffectService.aidl
 * Brief: This is aidl interface
 *
 * Author: Adam chen
 * Date: 109/11/27
 */
 
package custdroid.hardware;

import custdroid.hardware.IServiceCallBack;

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
