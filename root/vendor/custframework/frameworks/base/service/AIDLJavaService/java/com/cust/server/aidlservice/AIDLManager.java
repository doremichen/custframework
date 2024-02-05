/**
 *  Copyright (C) 2023 The Altek medical Project
 */
package com.cust.server.aidlservice;

import 	java.lang.RuntimeException;

import com.cust.framework.util.Print;

/**
 * AIDLManager
 */
public final class AIDLManager {
    private static final String TAG = AIDLManager.class.getSimpleName();
    
    // load jni so file
    static {
        System.loadLibrary("aidlservice_jni");
    }
    
    private static class Singleton {
        private static final AIDLManager sINSTANCE = new AIDLManager();
    }
    
    public static AIDLManager newInstance() {
        Print.i(TAG, "get AIDLManager object");
        return Singleton.sINSTANCE;
    }

    private AIDLManager() {
        Print.i(TAG, "AIDLManager construct!!!");
    }

    public interface onAIDLCallerListener {
        void onState(String message);
    }

    private onAIDLCallerListener mListener;

  
    public void setOnAIDLCallerListner(onAIDLCallerListener listener) {
        Print.i(TAG, "setOnAIDLCallerListner");
        Print.i(TAG, "listener: " + listener);
        this.mListener = listener;
        Print.i(TAG, "setOnAIDLCallerListner successful...");
    }
    
    public void init() {
        Print.i(TAG, "button device manager init!!!");
        int ret = native_init();
        if (ret == -1) {
            throw new RuntimeException("adc button initial exception!!!!");
        }
    }
    
    private void invokeListener(String msg) {
        Print.i(TAG, "invokeListener");
        // check listener validity
        if (this.mListener == null) {
            throw new RuntimeException("No listner!!! please check set listener state first!!!");
        }
        // call back
        this.mListener.onState(msg);
        
    }

    // -----------------------------------------------------------------
    // method

    
    /**
     * jni method section
     */
    private native int native_init();
    
    /**
     * invoke from jni
     */
     private void notifyFromNative(String msg) {
         Print.i(TAG, "[notifyFromNative]: msg = " + msg);
         
         // notify client
         invokeListener(msg);
     }
}
