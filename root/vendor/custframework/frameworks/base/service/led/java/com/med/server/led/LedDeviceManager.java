/**
 *  Copyright (C) xxxx
 */
package com.med.server.led;

import android.content.Context;
import android.content.Intent;
import android.os.RemoteCallbackList;
import android.os.RemoteException;

import com.med.hpframework.hardware.led.ILedCallback;
import com.med.hpframework.util.HpLog;

/**
 * LedDeviceManager manages EndoScope state in device mode.
 */
public final class LedDeviceManager {
    private static final String TAG = LedDeviceManager.class.getSimpleName();
    
    // load jni so file
    static {
        System.loadLibrary("led_jni");
    }

    private Context mContext;

    public LedDeviceManager(Context context) {
        this.mContext = context;
        HpLog.i(TAG, "led device manager start!!!");
    }

    // remote callback list
    private static final RemoteCallbackList<ILedCallback> sCallBacks = new RemoteCallbackList<ILedCallback>();
    
    
    public void registerCB(ILedCallback cb) {
        HpLog.i(TAG, "registerCB");
        HpLog.i(TAG, "cb: " + cb);
        boolean result = sCallBacks.register(cb);
        if (result == false) {
            HpLog.e(TAG, "register fail!!!");
            return;
        }
        HpLog.i(TAG, "registerCB successful...");
    }
    
    public void unregisterCB(ILedCallback cb) {
        HpLog.i(TAG, "unregisterCB");
        HpLog.i(TAG, "cb: " + cb);
        boolean result = sCallBacks.unregister(cb);
        if (result == false) {
            HpLog.e(TAG, "unregister fail!!!");
            return;
        }
        HpLog.i(TAG, "unregisterCB successful...");
    }
    
    public void triggerCB(String msg) {
        HpLog.i(TAG, "triggerCB");
        HpLog.i(TAG, "sCallBacks: " + sCallBacks);
        // check count of callback
        int number = sCallBacks.getRegisteredCallbackCount();
        if (number == 0) {
            throw new RuntimeException("No callbacks!!!");
        }

        final int N = sCallBacks.beginBroadcast();
        for (int i = 0; i < N; i++) {
            try {
                sCallBacks.getBroadcastItem(i).info(msg);
            } catch (RemoteException e) {
                /**
                 * Dead object occured!!!
                 */
            }
        }
        // finish broadcast
        sCallBacks.finishBroadcast();
    }

    // -----------------------------------------------------------------
    // method
    public int foo() {
        HpLog.i(TAG, "[foo] enter");
        // call jni function
        int ret = _foo();
        if (ret == -1) {
            // notify device error
            throw new RuntimeException("get result from jni is invalid!!!");
        }
        
        return ret;
    }

    
    /**
     * jni method section
     */
    private native int _foo();
    
    /**
     * invoke from jni
     */
     private void notifyFromNative(String msg) {
         HpLog.i(TAG, "[notifyFromNative]: msg = " + msg);
         
         // notify client
         triggerCB(msg);
     }
}
