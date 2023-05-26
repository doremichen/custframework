/**
 * File: SpecialEffectManager.java
 * Breif: Facade controller
 *
 * Name: 
 * Date: 
 */

package com.med.hpframework.hardware;

import android.os.Binder;
import android.os.RemoteException;
import android.os.ServiceManager;

import com.med.hpframework.util.HpLog;
import com.med.hpframework.hardware.ISpecialEffectService;
import com.med.hpframework.hardware.IServiceCallBack;

/**
 * Class that lets you access the Custdroid SpecialEffectsService.
 */
public final class SpecialEffectManager
{
    
    private static final String TAG = SpecialEffectManager.class.getSimpleName();
    
    // Binder proxy
    private ISpecialEffectService mBpSPEService;
    
    // Status listener
    private OnStatusListener mStatusListener;
    
    // Service callback
    private ServiceCallBackImp mServiceCB = new ServiceCallBackImp();
    
    private static class Helper {
        private static final SpecialEffectManager sINSTANCE = new SpecialEffectManager();
    }
    
    /**
     * Get SpecialEffectManager instance
     */
     public static SpecialEffectManager getInstance() {
         return Helper.sINSTANCE;
     }
    
    /**
     * Singleton
     */
     private SpecialEffectManager() {
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
      private class ServiceCallBackImp extends IServiceCallBack.Stub {
          
          @Override
          public void info(String msg) {
                long ident = Binder.clearCallingIdentity();
                try {
                    // notify UI
                    SpecialEffectManager.this.mStatusListener.onInfo(msg);
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
          mBpSPEService = ISpecialEffectService.Stub.asInterface(
                             ServiceManager.getService("cust.java.spe.sevice"));
          if (!checkServiceProxyValid()) {
              // notify UI
              listener.onError("No binder proxy");
              return;
          }
          
          // register callback
          try {
            mBpSPEService.registerCallBack(mServiceCB);
          } catch (RemoteException e) {
            HpLog.e(TAG, "RemoteException registerCallBack fail: " + e);
          }
          
          try {
            mBpSPEService.SpecialEffectInit();
          } catch (RemoteException e) {
            HpLog.e(TAG, "RemoteException init fail: " + e);
         }
      }
     
    
    /**
     * Turn on screen light
     */
    public boolean turnOn() {
        HpLog.i(TAG, "[turnOn] enter");
        if (!checkServiceProxyValid()) {
            // notify UI
            this.mStatusListener.onError("No binder proxy");
            return false;
        }
        boolean ret = false;
        try {
            ret = mBpSPEService.SpecialEffectsOn();
        } catch (RemoteException e) {
            HpLog.e(TAG, "RemoteException turn on fail: " + e);
        }
        return ret;
    }
    
    /**
     * Turn off screen light
     */
     public boolean turnOff() {
        HpLog.i(TAG, "[turnOff] enter");
        if (!checkServiceProxyValid()) {
            // notify UI
            this.mStatusListener.onError("No binder proxy");
            return false;
        }
        boolean ret = false;
        try {
            ret = mBpSPEService.SpecialEffectsOff();
        } catch (RemoteException e) {
            HpLog.e(TAG, "RemoteException turn off fail: " + e);
        }
        return ret;
     }
    
    
    /**
     * Release resource
     */
     public void release() {
        HpLog.i(TAG, "[release] enter");
        if (!checkServiceProxyValid()) {
            // notify UI
            this.mStatusListener.onError("No binder proxy");
            return;
        }
        
        try {
            mBpSPEService.release();
        } catch (RemoteException e) {
            HpLog.e(TAG, "RemoteException release fail: " + e);
        }
        
        // unregister callback
        try {
            mBpSPEService.unregisterCallBack(mServiceCB);
        } catch (RemoteException e) {
            HpLog.e(TAG, "RemoteException unregisterCallBack fail: " + e);
        }
     }
    
    /**
     * Binder proxy validity check
     */
    private boolean checkServiceProxyValid() {
        HpLog.i(TAG, "[checkServiceProxyValid] enter");
        return (mBpSPEService == null)? false: true;
    }
    
}
