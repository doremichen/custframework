/**
 * File: LedManager.java
 * Breif: Led API
 *
 * Name: 
 * Date: 
 */

package com.med.hpframework.hardware.led;

import android.os.Binder;
import android.os.RemoteException;
import android.os.ServiceManager;

import com.med.hp.server.HpSystemServer;
import com.med.hpframework.util.HpLog;
import com.med.hpframework.hardware.led.ILed;
import com.med.hpframework.hardware.led.ILedCallback;

/**
 * Class that lets you access the EndoScope service.
 */
public final class LedManager
{
    
    private static final String TAG = LedManager.class.getSimpleName();
    
    // Binder proxy
    private ILed mBpLedService;
    
    // Status listener
    private OnStatusListener mStatusListener;
    
    // Service callback
    private ServiceCallBackImp mServiceCB = new ServiceCallBackImp();
    
    private static class Singleton {
        private static final LedManager sINSTANCE = new LedManager();
    }
    
    /**
     * Get SpecialEffectManager instance
     */
     public static LedManager getInstance() {
         return Singleton.sINSTANCE;
     }
    
    /**
     * Singleton
     */
     private LedManager() {
     }
     
     /**
      * Listener interface
      */
      public interface OnStatusListener {
          void onError(String str);
          void onInfo(String str);
      }

     /**
      * Recieve the response from server
      */
      private class ServiceCallBackImp extends ILedCallback.Stub {
          
          @Override
          public void info(String msg) {
                long ident = Binder.clearCallingIdentity();
                try {
                    // notify UI
                    LedManager.this.mStatusListener.onInfo(msg);
                } finally {
                    Binder.restoreCallingIdentity(ident);
                }
          }
      }

     /**
      * Initialize device hal module
      */
      public void init(OnStatusListener listener) {
          HpLog.i(TAG, "[init] enter");
          this.mStatusListener = listener;
          mBpLedService = ILed.Stub.asInterface(
                             ServiceManager.getService(HpSystemServer.NAME.LED));
          if (!checkServiceProxyValid()) {
              // notify UI
              if (listener != null) {
                  listener.onError("No binder proxy");
              }
              return;
          }
          
          // register callback
          try {
            mBpLedService.registerCallBack(mServiceCB);
          } catch (RemoteException e) {
            HpLog.e(TAG, "RemoteException registerCallBack fail: " + e);
          }
          
      }
      
      /**
       * unregister the callback interface.
       */
      public void release(OnStatusListener listener) {
            HpLog.i(TAG, "[release] enter");
            if (!checkServiceProxyValid()) {
                // notify UI
                if (listener != null) {
                  listener.onError("No binder proxy");
                }
                return;
            }
            // unregister callback
            try {
                mBpLedService.unregisterCallBack(mServiceCB);
            } catch (RemoteException e) {
                HpLog.e(TAG, "RemoteException unregisterCallBack fail: " + e);
            }
      }
     
    
    /**
     * get foo function
     */
    public int foo() {
        HpLog.i(TAG, "[foo] enter");
        int ret = -1;
        if (!checkServiceProxyValid()) {
            // notify UI
            if (this.mStatusListener != null) {
                this.mStatusListener.onError("No binder proxy");
            }
            return ret;
        }

        try {
            ret = mBpLedService.foo();
        } catch (RemoteException e) {
            HpLog.e(TAG, "RemoteException get led id fail: " + e);
        }
        
        return ret;
    }
    
    
    /**
     * Binder proxy validity check
     */
    private boolean checkServiceProxyValid() {
        HpLog.i(TAG, "[checkServiceProxyValid] enter");
        return (mBpLedService == null)? false: true;
    }
    
}
