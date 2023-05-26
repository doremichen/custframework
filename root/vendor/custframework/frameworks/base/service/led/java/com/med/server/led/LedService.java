/**
 * Copyright (C) xxxx
 */
package com.med.server.led;

import android.content.Context;
import android.os.Binder;

import com.med.hpframework.hardware.led.ILed;
import com.med.hpframework.hardware.led.ILedCallback;
import com.med.hpframework.util.HpLog;


/**
 * HeliosPro LedService
 */
public final class LedService extends ILed.Stub {


    private static final String TAG = LedService.class.getSimpleName();

    private LedDeviceManager mDeviceManager;


    public LedService(Context context) {
       HpLog.i(TAG, "LedService startup");
       
       // start to watch led state
       this.mDeviceManager = new LedDeviceManager(context);
       
    }
    
    /**
     * Initialize cust hal module
     * 
     * @return Success if 0 otherwise Fail
     */
    @Override
    public int foo() {
       HpLog.i(TAG, "foo");
       int ret = -1;
       long ident = Binder.clearCallingIdentity();
       try {
           // call led device manager function
           ret = this.mDeviceManager.foo();
       } finally {
           Binder.restoreCallingIdentity(ident);
       }
       
       return ret;
    }
    

    @Override
    public void registerCallBack(ILedCallback callback) {
        HpLog.i(TAG, "registerCallBack callback = " + callback.getClass().getSimpleName());
        long ident = Binder.clearCallingIdentity();
        try {
            if (callback != null) this.mDeviceManager.registerCB(callback);
        } finally {
           Binder.restoreCallingIdentity(ident);
        }
        
    }
    
    @Override
    public void unregisterCallBack(ILedCallback callback) {
        HpLog.i(TAG, "unregisterCallBack callback = " + callback.getClass().getSimpleName());
        long ident = Binder.clearCallingIdentity();
        try {
            if (callback != null) this.mDeviceManager.unregisterCB(callback);
        } finally {
           Binder.restoreCallingIdentity(ident);
        }
    }
 
}
