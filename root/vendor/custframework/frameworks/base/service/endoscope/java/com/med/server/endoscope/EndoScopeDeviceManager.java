/**
 *  Copyright (C) xxxx
 */
package com.med.server.endoscope;

import android.content.Context;
import android.content.Intent;
import android.os.RemoteCallbackList;
import android.os.RemoteException;
import android.os.UEventObserver;
import android.os.UserHandle;

import com.med.hpframework.hardware.endoscope.IEndoscopeCallback;
import com.med.hpframework.util.HpLog;

import java.util.HashMap;
import java.util.Map;

/**
 * EndoScopeDeviceManager manages EndoScope state in device mode.
 */
public final class EndoScopeDeviceManager {
    private static final String TAG = EndoScopeDeviceManager.class.getSimpleName();
    
    // load jni so file
    static {
        System.loadLibrary("endoscope_jni");
    }
    

    // TODO: Need to modify when the endoscope uevent path is defined.
    private static final String EndoScope_STATE_MATCH =
            "DEVPATH=/devices/virtual/android_usb/android0";

    public static final String ACTION_ENDOSCOPE_STATE =
            "com.med.hpframework.hardware.endoscope.state";
    public static final String ENDOSCOPE_CONNECTED = "connected";

    private Context mContext;

    private EndoScopeUeventObserver mUEventObserver;

    public EndoScopeDeviceManager(Context context) {
        this.mContext = context;
        HpLog.i(TAG, "start to watch endoscope state!!!");
        // watch the endoscope state
        this.mUEventObserver = new EndoScopeUeventObserver();
        this.mUEventObserver.startObserving(EndoScope_STATE_MATCH);
    }

    // remote callback list
    private static final RemoteCallbackList<IEndoscopeCallback> sCallBacks = new RemoteCallbackList<IEndoscopeCallback>();
    
    
    public void registerCB(IEndoscopeCallback cb) {
        HpLog.i(TAG, "registerCB");
        HpLog.i(TAG, "cb: " + cb);
        boolean result = sCallBacks.register(cb);
        if (result == false) {
            HpLog.e(TAG, "register fail!!!");
            return;
        }
        HpLog.i(TAG, "registerCB successful...");
    }
    
    public void unregisterCB(IEndoscopeCallback cb) {
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
    public int getEndoScopeId() {
        HpLog.i(TAG, "[getEndoScopeId] enter");
        // call jni function
        int ret = _getEndoScopeId();
        if (ret == -1) {
            // notify device error
            throw new RuntimeException("Get endoscope id fail!!!");
        }
        
        return ret;
    }


    /*
     * Listens for uevent messages from the kernel to monitor the EndoScope state
     */
    private final class EndoScopeUeventObserver extends  UEventObserver {

        private boolean mConnected;

        private static final String CONNECTED = "CONNECTED";
        private static final String DISCONNECTED = "DISCONNECTED";

        private Map<String, Boolean> mMap = new HashMap<>();
        {
            this.mMap.put("CONNECTED", true);
            this.mMap.put("DISCONNECTED", false);
        }

        @Override
        public void onUEvent(UEventObserver.UEvent event) {
            HpLog.v(TAG, "USB UEVENT: " + event.toString());
            /**
             * state: CONNECTED, DISCONNECTED, UNKOWN
             */
             // TODO: Need to modify when the endoscope state is defined.
            String state = event.get("USB_STATE");
            if (state != null) {
                HpLog.i(TAG, "EndoScope state is changed...");
                Boolean tmp = this.mMap.get(state);
                // unkown check
                if (tmp == null) {
                    this.mConnected = false;
                } else {
                    this.mConnected = tmp;
                }
            }
            // send broadcast
            updateEndoSCopeStateBroadcastIfNeeded();
        }

        /**
         *
         */
        protected void updateEndoSCopeStateBroadcastIfNeeded() {
            HpLog.i(TAG, "updateEndoSCopeStateBroadcastIfNeeded enter");
            Intent intent = new Intent(ACTION_ENDOSCOPE_STATE);
            intent.addFlags(Intent.FLAG_RECEIVER_REPLACE_PENDING
                    | Intent.FLAG_RECEIVER_REGISTERED_ONLY);
            intent.putExtra(ENDOSCOPE_CONNECTED, this.mConnected);
            
            sendStickyBroadcast(intent);
        }
        
        protected void sendStickyBroadcast(Intent intent) {
            HpLog.i(TAG, "sendStickyBroadcast enter");
            mContext.sendStickyBroadcastAsUser(intent, UserHandle.ALL);
        }
    }
    
    
    /**
     * jni method section
     */
    private native int _getEndoScopeId();
    
    /**
     * invoke from jni
     */
     private void notifyFromNative(String msg) {
         HpLog.i(TAG, "[notifyFromNative]: msg = " + msg);
         
         // notify client
         triggerCB(msg);
     }
}
