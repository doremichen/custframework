/**
 *  Copyright (C) 2023 The Altek medical Project
 */
package com.cust.server.aidlservice;

import 	java.lang.RuntimeException;

import android.content.Context;
import android.content.Intent;
import android.os.RemoteCallbackList;
import android.os.RemoteException;

import com.cust.framework.hardware.aidlSevice.IAIDLJavaServiceCallback;
import com.cust.framework.util.Print;

/**
 * AIDLManagerEx
 */
public final class AIDLManagerEx {
    private static final String TAG = AIDLManagerEx.class.getSimpleName();
    
    private Context mContext;

    public AIDLManagerEx(Context context) {
        this.mContext = context;
        Print.i(TAG, "aldl java service manager start!!!");
    }

    
    public void start() {
        Print.i(TAG, "start to detcect button device manager!!!");
        AIDLManager.newInstance().init();
    }
    
    public void testCallback() {
        AIDLManager.newInstance().testCBfromNative();
        //invokeListener("I am call back test!!!");
    }
    
    private class AIDLJavaServiceCallback implements AIDLManager.onAIDLCallerListener {
        
        @Override
        public void onState(String message) {
            invokeListener(message);
        }
    }
    
    private AIDLJavaServiceCallback mCallback = new AIDLJavaServiceCallback();
    
    
    // remote callback list
    private static final RemoteCallbackList<IAIDLJavaServiceCallback> sCallBacks = new RemoteCallbackList<IAIDLJavaServiceCallback>();
    
   
    public void registerCB(IAIDLJavaServiceCallback cb) {
        Print.i(TAG, "registerCB");
        Print.i(TAG, "cb: " + cb);
        boolean result = sCallBacks.register(cb);
        if (result == false) {
            throw new RuntimeException("register fail!!!!");
        }
        
        // register button device manager
        AIDLManager.newInstance().setOnAIDLCallerListner(mCallback);
        
        Print.i(TAG, "registerCB successful...");
    }
    
    public void unregisterCB(IAIDLJavaServiceCallback cb) {
        Print.i(TAG, "unregisterCB");
        Print.i(TAG, "cb: " + cb);
        boolean result = sCallBacks.unregister(cb);
        if (result == false) {
            throw new RuntimeException("unregister fail!!!!");
        }
        
        // unregister button device manager
        AIDLManager.newInstance().setOnAIDLCallerListner(null);
        
        Print.i(TAG, "unregisterCB successful...");
    }
    
    public void invokeListener(String msg) {
        Print.i(TAG, "invokeListener");
        Print.i(TAG, "sCallBacks: " + sCallBacks);
        // check count of callback
        int number = sCallBacks.getRegisteredCallbackCount();
        if (number == 0) {
            throw new RuntimeException("No callbacks!!!");
        }

        final int N = sCallBacks.beginBroadcast();
        for (int i = 0; i < N; i++) {
            try {
                sCallBacks.getBroadcastItem(i).onState(msg);
            } catch (RemoteException e) {
                /**
                 * Dead object occured!!!
                 */
                throw new RuntimeException("Dead object occured: " + e.getMessage());
            }
        }
        // finish broadcast
        sCallBacks.finishBroadcast();
    }

}
