/**
 * Copyright (C) xxxx
 */
package com.med.server.endoscope;

import android.content.Context;
import android.os.Binder;

import com.med.hpframework.hardware.endoscope.IEndoscope;
import com.med.hpframework.hardware.endoscope.IEndoscopeCallback;
import com.med.hpframework.util.HpLog;


/**
 * HeliosPro EndoScopeService
 */
public final class EndoScopeService extends IEndoscope.Stub {


    private static final String TAG = EndoScopeService.class.getSimpleName();

    private EndoScopeDeviceManager mDeviceManager;


    public EndoScopeService(Context context) {
       HpLog.i(TAG, "EndoScopeService startup");
       
       // start to watch endoscope state
       this.mDeviceManager = new EndoScopeDeviceManager(context);
       
    }
    
    /**
     * Initialize cust hal module
     * 
     * @return Success if 0 otherwise Fail
     */
    @Override
    public int getEndoScopeId() {
       HpLog.i(TAG, "getEndoScopeId");
       int ret = -1;
       long ident = Binder.clearCallingIdentity();
       try {
           // call endoscope device manager function
           ret = this.mDeviceManager.getEndoScopeId();
       } finally {
           Binder.restoreCallingIdentity(ident);
       }
       
       return ret;
    }
    

    @Override
    public void registerCallBack(IEndoscopeCallback callback) {
        HpLog.i(TAG, "registerCallBack callback = " + callback.getClass().getSimpleName());
        long ident = Binder.clearCallingIdentity();
        try {
            if (callback != null) this.mDeviceManager.registerCB(callback);
        } finally {
           Binder.restoreCallingIdentity(ident);
        }
        
    }
    
    @Override
    public void unregisterCallBack(IEndoscopeCallback callback) {
        HpLog.i(TAG, "unregisterCallBack callback = " + callback.getClass().getSimpleName());
        long ident = Binder.clearCallingIdentity();
        try {
            if (callback != null) this.mDeviceManager.unregisterCB(callback);
        } finally {
           Binder.restoreCallingIdentity(ident);
        }
    }
 
}
