/**
 *  Copyright (C) xxxx
 */
package com.med.server.adc;

import 	java.lang.RuntimeException;

import com.med.hpframework.util.HpLog;

/**
 * ButtonDeviceManager
 */
public final class ButtonDeviceManager {
    private static final String TAG = ButtonDeviceManager.class.getSimpleName();
    
    // load jni so file
    static {
        System.loadLibrary("adc_button_jni");
    }
    
    private static class Singleton {
        private static final ButtonDeviceManager sINSTANCE = new ButtonDeviceManager();
    }
    
    public static ButtonDeviceManager newInstance() {
        HpLog.i(TAG, "get ButtonDeviceManager object");
        return Singleton.sINSTANCE;
    }

    private ButtonDeviceManager() {
        HpLog.i(TAG, "ButtonDeviceManager construct!!!");
    }

    // button event
    public interface BUTTON_EVENT {
        String SW_RELEASED = "SW_RELEASED";
        String SW1_PRESSED = "SW1_PRESSED";
        String SW2_PRESSED = "SW2_PRESSED";
        String ALL_SW_PRESSED = "ALL_SW_PRESSED";
        String SW_UNKNOWN = "SW_UNKNOWN";
    }

    public interface onADCButtonListener {
        public void onState(String button_event);
    }

    private onADCButtonListener mListener;

  
    public void setOnADCButtonListner(onADCButtonListener listener) {
        HpLog.i(TAG, "setOnADCButtonListner");
        HpLog.i(TAG, "listener: " + listener);
        this.mListener = listener;
        HpLog.i(TAG, "setOnADCButtonListner successful...");
    }
    
    public void init() {
        HpLog.i(TAG, "button device manager init!!!");
        int ret = native_init();
        if (ret == -1) {
            throw new RuntimeException("adc button initial exception!!!!");
        }
    }
    
    private void invokeListener(String msg) {
        HpLog.i(TAG, "invokeListener");
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
         HpLog.i(TAG, "[notifyFromNative]: msg = " + msg);
         
         // notify client
         invokeListener(msg);
     }
}
