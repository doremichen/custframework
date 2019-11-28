/*
 * File: ISpecialEffectService.aidl
 * Brief: This is aidl interface
 *
 * Author: Adam chen
 * Date: 109/11/27
 */
 
package custdroid.hardware;


/**
 * {@hide}
 */
interface ISpecialEffectService
{
   boolean SpecialEffectInit();
   boolean SpecialEffectsOn();
   boolean SpecialEffectsOff();
   void release();
}
