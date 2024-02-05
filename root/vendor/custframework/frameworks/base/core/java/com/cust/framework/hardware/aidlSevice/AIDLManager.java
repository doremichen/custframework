/**
 *  Copyright (C) 2023 The Altek custical Project
 */
package com.cust.framework.hardware.aidlSevice;

import android.os.Binder;
import android.os.RemoteException;
import android.os.ServiceManager;

import com.cust.system.server.HpSystemServer;
import com.cust.framework.util.Print;
import com.cust.framework.hardware.aidlSevice.IAIDLJavaService;
import com.cust.framework.hardware.aidlSevice.IAIDLJavaServiceCallback;

/**
 * Provider the interface that access adc button service
 */
public final class AIDLManager
{
    
    private static final String TAG = AIDLManager.class.getSimpleName();
    
    // Binder proxy
    private IAIDLJavaService mBpAIDLJavaService;
    
    // Status listener
    private onAIDLSeviceListener mListener;
        
    
    // Service callback
    private ServiceCallBackImp mServiceCB = new ServiceCallBackImp();
    
    private static class Singleton {
        private static final AIDLManager sINSTANCE = new AIDLManager();
    }
    
    /**
     * Get SpecialEffectManager instance
     */
    public static AIDLManager getInstance() {
         return Singleton.sINSTANCE;
    }
    
    /**
     * Singleton
     */
    private AIDLManager() {
        
    }
     
    public interface onAIDLSeviceListener {
        void onState(String button_event);
        void onError(String msg);
    }
     
    /**
    * Recieve the response from server
    */
    private class ServiceCallBackImp extends IAIDLJavaServiceCallback.Stub {
        
        @Override
        public void onState(String msg) {
            Print.i(TAG, "[onState] +++");
            long ident = Binder.clearCallingIdentity();
            try {
                // precheck
                if (AIDLManager.this.mListener == null) {
                    Print.i(TAG, "No aidlSevice listener!!!");
                    return;
                }
                // notify UI
                AIDLManager.this.mListener.onState(msg);
            } finally {
                Binder.restoreCallingIdentity(ident);
            }
            Print.i(TAG, "[onState] xxx");
        }
    }

    /**
     * set adc button listerner and get adc service proxy
     */
    public void init(onAIDLSeviceListener listener) {
        Print.i(TAG, "[init] enter");
        this.mListener = listener;
        mBpAIDLJavaService = IAIDLJavaService.Stub.asInterface(
                           ServiceManager.getService(HpSystemServer.NAME.AIDLService));
        if (!checkServiceProxyValid()) {
            // notify UI
            if (listener != null) {
                listener.onError("No binder proxy");
            }
            return;
        }
        
        // register callback
        try {
          mBpAIDLJavaService.registerCallBack(mServiceCB);
        } catch (RemoteException e) {
          Print.e(TAG, "RemoteException registerCallBack fail: " + e);
        }
        
    }
    
    /**
     * unregister the callback interface.
     */
    public void release(onAIDLSeviceListener listener) {
          Print.i(TAG, "[release] enter");
          if (!checkServiceProxyValid()) {
              // notify UI
              if (listener != null) {
                listener.onError("No binder proxy");
              }
              return;
          }
          // unregister callback
          try {
              mBpAIDLJavaService.unregisterCallBack(mServiceCB);
          } catch (RemoteException e) {
              Print.e(TAG, "RemoteException unregisterCallBack fail: " + e);
          }
    }
    
    
    /**
     * start detect adc button
     */
    public void start() {
        Print.i(TAG, "[init] enter");
        if (!checkServiceProxyValid()) {
            // notify UI
            if (this.mListener != null) {
                this.mListener.onError("No binder proxy");
            }
            return;
        }

        try {
            mBpAIDLJavaService.start();
        } catch (RemoteException e) {
            Print.e(TAG, "RemoteException get aidlSevice id fail: " + e);
        }
    }
    
    
    public void testCB() {
        Print.i(TAG, "[testCB] enter");
        if (!checkServiceProxyValid()) {
            // notify UI
            if (this.mListener != null) {
                this.mListener.onError("No binder proxy");
            }
            return;
        }

        try {
            mBpAIDLJavaService.testCallback();
        } catch (RemoteException e) {
            Print.e(TAG, "RemoteException get aidlSevice id fail: " + e);
        }
    }
    
    /**
     * Binder proxy validity check
     */
    private boolean checkServiceProxyValid() {
        Print.i(TAG, "[checkServiceProxyValid] enter");
        return (mBpAIDLJavaService == null)? false: true;
    }
    
}
