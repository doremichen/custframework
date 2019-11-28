/**
 * File: SpecialEffectsService.java
 * Breif: Jni method controller
 *
 * Name: Adam chen
 * Date: 
 */

package com.cust.server;

import android.content.Context;
import android.os.Binder;
import android.os.ParcelFileDescriptor;
import android.os.RemoteException;
import android.os.IBinder;
import android.util.Config;

import custdroid.hardware.ISpecialEffectService;
import custdroid.util.Print;

import java.util.ArrayList;


/**
 * JNI interface
 */

public final class SpecialEffectsService extends ISpecialEffectService.Stub {

    // load jni so file
    static {
        System.loadLibrary("cust_jni");
    }

    public SpecialEffectsService() {
       Print.info(this, "SpecialEffectsService startup");
    }
    
    /**
     * Initialize cust hal module
     * 
     * @return Success if 0 otherwise Fail
     */
    @Override
    public boolean SpecialEffectInit() {
       Print.info(this, "SpecialEffectInit");
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
        Print.info(this, "SpecialEffectsOn");
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
        Print.info(this, "SpecialEffectsOff");
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
     * @return Success if 0 otherwise Fail
     */
    @Override
    public void release() {
        Print.info(this, "release");
        long ident = Binder.clearCallingIdentity();
        int ret;
        try {
            _special_effects_release();
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
         Print.info(this, "[notifyFromNative]: msg = " + msg);
     }
}
