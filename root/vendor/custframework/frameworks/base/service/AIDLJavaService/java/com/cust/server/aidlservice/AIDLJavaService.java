/**
 *  Copyright (C) 2023 The Altek medical Project
 */
package com.cust.server.aidlservice;

import android.content.Context;
import android.os.Binder;

import com.cust.framework.hardware.aidlSevice.IAIDLJavaService;
import com.cust.framework.hardware.aidlSevice.IAIDLJavaServiceCallback;
import com.cust.framework.util.Print;


/**
 * adc button Service
 */
public final class AIDLJavaService extends IAIDLJavaService.Stub {

    private static final String TAG = AIDLJavaService.class.getSimpleName();

    private AIDLManagerEx mAIDLManager;

    public AIDLJavaService(Context context) {
       Print.i(TAG, "AIDLJavaService startup");
       
       // access to jni
       this.mAIDLManager = new AIDLManagerEx(context);
       
    }
    
    /**
     * 
     * Start dectect adc button
     * 
     */
    @Override
    public void start() {
       Print.i(TAG, "start");
       long ident = Binder.clearCallingIdentity();
       try {
           // 
           this.mAIDLManager.start();
       } finally {
           Binder.restoreCallingIdentity(ident);
       }
    }
    
    @Override
    public void testCallback() {
        long ident = Binder.clearCallingIdentity();
        
        try {
           // 
           this.mAIDLManager.testCallback();
       } finally {
           Binder.restoreCallingIdentity(ident);
       }      
    }

    @Override
    public void registerCallBack(IAIDLJavaServiceCallback callback) {
        Print.i(TAG, "registerCallBack callback = " + callback.getClass().getSimpleName());
        long ident = Binder.clearCallingIdentity();
        try {
            if (callback != null) this.mAIDLManager.registerCB(callback);
        } finally {
           Binder.restoreCallingIdentity(ident);
        }
        
    }
    
    @Override
    public void unregisterCallBack(IAIDLJavaServiceCallback callback) {
        Print.i(TAG, "unregisterCallBack callback = " + callback.getClass().getSimpleName());
        long ident = Binder.clearCallingIdentity();
        try {
            if (callback != null) this.mAIDLManager.unregisterCB(callback);
        } finally {
           Binder.restoreCallingIdentity(ident);
        }
    }
 
}
