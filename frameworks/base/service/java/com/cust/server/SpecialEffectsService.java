package com.cust.server;

import android.content.Context;
import android.os.Binder;
import android.os.ParcelFileDescriptor;
import android.os.RemoteException;
import android.os.IBinder;
import android.util.Config;
import android.util.Log;

import java.util.ArrayList;


/**
 * Class that manages the device's sensors. It register clients and activate
 * the needed sensors. The sensor events themselves are not broadcasted from
 * this service, instead, a file descriptor is provided to each client they
 * can read events from.
 */

public final class SpecialEffectsService {
    static final String TAG = SpecialEffectsService.class.getSimpleName();
    private static final boolean DEBUG = true;


    static {
        System.loadLibrary("cust_jni");
    }

   
    @SuppressWarnings("unused")
    public SpecialEffectsService(Context context) {
       PrintI("SpecialEffectsService startup");
    }
    
    public boolean SpecialEffectInit() {
       PrintI("SpecialEffectInit");
       int ret = _special_effects_init();
       
       return true;   
        
    }
    
    public boolean SpecialEffectsOn() {
        PrintI("SpecialEffectsOn");
        int ret = _special_effects_on();
        return (ret<0) ? false : true;
    }
    
    public boolean SpecialEffectsOff() {
        PrintI("SpecialEffectsOff");
        int ret = _special_effects_off();
        return (ret<0) ? false : true;
    }
    
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
