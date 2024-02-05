/**
 * File: SpecialEffectManager.java
 * Breif: Facade controller
 *
 * Name: 
 * Date: 
 */

package com.cust.framework.hardware.specialEffect;

import android.os.Binder;
import android.os.RemoteException;
import android.os.ServiceManager;

import com.cust.framework.util.Print;
import com.cust.framework.hardware.specialEffect.ISpecialEffectService;
import com.cust.framework.hardware.specialEffect.IServiceCallBack;

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
          Print.i(TAG, "[init] enter");
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
            Print.e(TAG, "RemoteException registerCallBack fail: " + e);
          }
          
          try {
            mBpSPEService.SpecialEffectInit();
          } catch (RemoteException e) {
            Print.e(TAG, "RemoteException init fail: " + e);
         }
      }
     
    
    /**
     * Turn on screen light
     */
    public boolean turnOn() {
        Print.i(TAG, "[turnOn] enter");
        if (!checkServiceProxyValid()) {
            // notify UI
            this.mStatusListener.onError("No binder proxy");
            return false;
        }
        boolean ret = false;
        try {
            ret = mBpSPEService.SpecialEffectsOn();
        } catch (RemoteException e) {
            Print.e(TAG, "RemoteException turn on fail: " + e);
        }
        return ret;
    }
    
    /**
     * Turn off screen light
     */
     public boolean turnOff() {
        Print.i(TAG, "[turnOff] enter");
        if (!checkServiceProxyValid()) {
            // notify UI
            this.mStatusListener.onError("No binder proxy");
            return false;
        }
        boolean ret = false;
        try {
            ret = mBpSPEService.SpecialEffectsOff();
        } catch (RemoteException e) {
            Print.e(TAG, "RemoteException turn off fail: " + e);
        }
        return ret;
     }
    
    
    /**
     * Release resource
     */
     public void release() {
        Print.i(TAG, "[release] enter");
        if (!checkServiceProxyValid()) {
            // notify UI
            this.mStatusListener.onError("No binder proxy");
            return;
        }
        
        try {
            mBpSPEService.release();
        } catch (RemoteException e) {
            Print.e(TAG, "RemoteException release fail: " + e);
        }
        
        // unregister callback
        try {
            mBpSPEService.unregisterCallBack(mServiceCB);
        } catch (RemoteException e) {
            Print.e(TAG, "RemoteException unregisterCallBack fail: " + e);
        }
     }
    
    /**
     * Binder proxy validity check
     */
    private boolean checkServiceProxyValid() {
        Print.i(TAG, "[checkServiceProxyValid] enter");
        return (mBpSPEService == null)? false: true;
    }
    
}
