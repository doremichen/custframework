/**
 * File: SpecialEffectManager.java
 * Breif: Facade controller
 *
 * Name: Adam chen
 * Date: 
 */

package custdroid.hardware;

import android.content.Context;
import android.os.Binder;
import android.os.Bundle;
import android.os.Parcelable;
import android.os.ParcelFileDescriptor;
import android.os.Process;
import android.os.RemoteException;
import android.os.Handler;
import android.os.Message;
import android.os.ServiceManager;
import android.util.Log;

import custdroid.hardware.ISpecialEffectService;

/**
 * Class that lets you access the Custdroid SpecialEffectsService.
 */
public final class SpecialEffectManager
{
    private static final String TAG = "SpecialEffectManager";
    private static final boolean DEBUG = true;
    
    // Binder proxy
    private ISpecialEffectService mBpSPEService;
    
    // Status listener
    private OnStatusListener mStatusListener;
    
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
         mBpSPEService = ISpecialEffectService.Stub.asInterface(
                             ServiceManager.getService("cust.java.spe.sevice"));
     }
     
     /**
      * Listener interface
      */
      public interface OnStatusListener {
          void onError(String str);
      }

     /**
      * INitialize device hal module
      */
      public void init(OnStatusListener listener) {
          PrintI("[turnOn] enter");
          this.mStatusListener = listener;
          if (!checkServiceProxyValid()) {
              // notify UI
              listener.onError("No binder proxy");
              return;
          }
          
          try {
            mBpSPEService.SpecialEffectInit();
          } catch (RemoteException e) {
            Log.e(TAG, "RemoteException init fail:", e);
         }
      }
     
    
    /**
     * Turn on screen light
     */
    public boolean turnOn() {
        PrintI("[turnOn] enter");
        if (!checkServiceProxyValid()) {
            // notify UI
            this.mStatusListener.onError("No binder proxy");
            return false;
        }
        boolean ret = false;
        try {
            ret = mBpSPEService.SpecialEffectsOn();
        } catch (RemoteException e) {
            Log.e(TAG, "RemoteException turn on fail:", e);
        }
        return ret;
    }
    
    /**
     * Turn off screen light
     */
     public boolean turnOff() {
        PrintI("[turnOff] enter");
        if (!checkServiceProxyValid()) {
            // notify UI
            this.mStatusListener.onError("No binder proxy");
            return false;
        }
        boolean ret = false;
        try {
            ret = mBpSPEService.SpecialEffectsOff();
        } catch (RemoteException e) {
            Log.e(TAG, "RemoteException turn off fail:", e);
        }
        return ret;
     }
    
    
    /**
     * Release resource
     */
     public void release() {
        PrintI("[release] enter");
        if (!checkServiceProxyValid()) {
            // notify UI
            this.mStatusListener.onError("No binder proxy");
            return;
        }

        try {
            mBpSPEService.release();
        } catch (RemoteException e) {
            Log.e(TAG, "RemoteException turn off fail:", e);
        }
     }
    
    /**
     * Binder proxy validity check
     */
    private boolean checkServiceProxyValid() {
        PrintI("[checkServiceProxyValid] enter");
        return (mBpSPEService == null)? false: true;
    }
    
    private static void PrintI(String str) {
        if(DEBUG) Log.i(TAG, str);   
        
    }
   
    
}
