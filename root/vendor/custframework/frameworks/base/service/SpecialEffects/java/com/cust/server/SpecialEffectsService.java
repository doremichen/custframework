/**
 * File: SpecialEffectsService.java
 * Breif: Jni method controller
 *
 * Name: 
 * Date: 
 */

package com.cust.server;

import android.os.Binder;
import android.os.RemoteCallbackList;
import android.os.RemoteException;

import com.med.hpframework.hardware.IServiceCallBack;
import com.med.hpframework.hardware.ISpecialEffectService;
import com.med.hpframework.util.HpLog;


/**
 * JNI interface
 */

public final class SpecialEffectsService extends ISpecialEffectService.Stub {


    private static final String TAG = SpecialEffectsService.class.getSimpleName();

    // load jni so file
    static {
        System.loadLibrary("cust_jni");
    }

    public SpecialEffectsService() {
       HpLog.i(TAG, "SpecialEffectsService startup");
    }
    
    /**
     * Hanlde register/unregister and callback 
     */
     private static enum ServiceHandler {
            INSTANCE;

         // remote callback list
         private final RemoteCallbackList<IServiceCallBack> mCallBacks = new RemoteCallbackList<IServiceCallBack>();
         
         
         public void registerCB(IServiceCallBack cb) {
             HpLog.i(TAG, "registerCB");
             mCallBacks.register(cb);
         }
         
         public void unregisterCB(IServiceCallBack cb) {
             HpLog.i(TAG, "unregisterCB");
             mCallBacks.unregister(cb);
         }
         
         public void triggerCB(String msg) {
             HpLog.i(TAG, "triggerCB");
             final int N = mCallBacks.beginBroadcast();
                     for (int i = 0; i < N; i++) {
                         try {
                             mCallBacks.getBroadcastItem(i).info(msg);
                         } catch (RemoteException e) {
                             /**
                              * Dead object occured!!!
                              */
                         }
                     }
                     // finish broadcast
                     mCallBacks.finishBroadcast();
         }
       
     }

    /**
     * Initialize cust hal module
     * 
     * @return Success if 0 otherwise Fail
     */
    @Override
    public boolean SpecialEffectInit() {
       HpLog.i(TAG, "SpecialEffectInit");
       long ident = Binder.clearCallingIdentity();
       int ret;
       try {
           ret = _special_effects_init();
       } finally {
           Binder.restoreCallingIdentity(ident);
       }
       
       return (ret != 0)? false: true;
        
    }
    
    /**
     * Turn on screen light
     * 
     * @return Success if 0 otherwise Fail
     */
    @Override
    public boolean SpecialEffectsOn() {
        HpLog.i(TAG, "SpecialEffectsOn");
        long ident = Binder.clearCallingIdentity();
        int ret;
        try {
          ret = _special_effects_on();
        } finally {
           Binder.restoreCallingIdentity(ident);
        }
        
        return (ret != 0)? false : true;
    }
    
    /**
     * Turn off screen light
     * 
     * @return Success if 0 otherwise Fail
     */
    @Override
    public boolean SpecialEffectsOff() {
        HpLog.i(TAG, "SpecialEffectsOff");
        long ident = Binder.clearCallingIdentity();
        int ret;
        try {
          ret = _special_effects_off();
        } finally {
           Binder.restoreCallingIdentity(ident);
        }
        return (ret != 0)? false : true;
    }
    
    /**
     * Release resource
     * 
     * 
     */
    @Override
    public void release() {
        HpLog.i(TAG, "release");
        long ident = Binder.clearCallingIdentity();
        int ret;
        try {
            _special_effects_release();
        } finally {
           Binder.restoreCallingIdentity(ident);
        }
    }
 
    @Override
    public void registerCallBack(IServiceCallBack callback) {
        HpLog.i(TAG, "registerCallBack callback = " + callback.getClass().getSimpleName());
        long ident = Binder.clearCallingIdentity();
        try {
            if (callback != null) ServiceHandler.INSTANCE.registerCB(callback);
        } finally {
           Binder.restoreCallingIdentity(ident);
        }
        
    }
    
    @Override
    public void unregisterCallBack(IServiceCallBack callback) {
        HpLog.i(TAG, "unregisterCallBack callback = " + callback.getClass().getSimpleName());
        long ident = Binder.clearCallingIdentity();
        try {
            if (callback != null) ServiceHandler.INSTANCE.unregisterCB(callback);
        } finally {
           Binder.restoreCallingIdentity(ident);
        }
    }
 
    /**
     * jni method section
     */
    private native final int _special_effects_init();
    private native final int _special_effects_on();
    private native final int _special_effects_off();
    private native final void _special_effects_release();
    
    /**
     * invoke from jni
     */
     private void notifyFromNative(String msg) {
         HpLog.i(TAG, "[notifyFromNative]: msg = " + msg);
         
         // notify client
         ServiceHandler.INSTANCE.triggerCB(msg);
     }
}
