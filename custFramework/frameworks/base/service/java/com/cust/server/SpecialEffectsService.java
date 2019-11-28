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
import android.util.Log;

import custdroid.hardware.ISpecialEffectService;

import java.util.ArrayList;


/**
 * JNI interface
 */

public final class SpecialEffectsService extends ISpecialEffectService.Stub {
    static final String TAG = SpecialEffectsService.class.getSimpleName();
    private static final boolean DEBUG = true;


    static {
        System.loadLibrary("cust_jni");
    }

    public SpecialEffectsService() {
       PrintI("SpecialEffectsService startup");
    }
    
    @Override
    public boolean SpecialEffectInit() {
       PrintI("SpecialEffectInit");
       int ret = _special_effects_init();
       
       return true;   
        
    }
    
    @Override
    public boolean SpecialEffectsOn() {
        PrintI("SpecialEffectsOn");
        int ret = _special_effects_on();
        return (ret<0) ? false : true;
    }
    
    @Override
    public boolean SpecialEffectsOff() {
        PrintI("SpecialEffectsOff");
        int ret = _special_effects_off();
        return (ret<0) ? false : true;
    }
    
    @Override
    public boolean release() {
        PrintI("release");
        int ret = _special_effects_release();
        
        return true;
    }    
 
    private void PrintI(String str) {
        
        if(DEBUG) Log.i(TAG, str);    
        
    }    
 
 
    private native final int _special_effects_init();
    private native final int _special_effects_on();
    private native final int _special_effects_off();
    private native final int _special_effects_release();
}
