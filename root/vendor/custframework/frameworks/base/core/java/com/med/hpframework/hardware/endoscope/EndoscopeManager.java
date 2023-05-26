/**
 * File: EndoscopeManager.java
 * Breif: EndoScope API
 *
 * Name: 
 * Date: 
 */

package com.med.hpframework.hardware.endoscope;

import android.os.Binder;
import android.os.RemoteException;
import android.os.ServiceManager;

import com.med.hp.server.HpSystemServer;
import com.med.hpframework.util.HpLog;
import com.med.hpframework.hardware.endoscope.IEndoscope;
import com.med.hpframework.hardware.endoscope.IEndoscopeCallback;

/**
 * Class that lets you access the EndoScope service.
 */
public final class EndoscopeManager
{
    
    private static final String TAG = EndoscopeManager.class.getSimpleName();
    
    // Binder proxy
    private IEndoscope mBpEndoScopeService;
    
    // Status listener
    private OnStatusListener mStatusListener;
    
    // Service callback
    private ServiceCallBackImp mServiceCB = new ServiceCallBackImp();
    
    private static class Singleton {
        private static final EndoscopeManager sINSTANCE = new EndoscopeManager();
    }
    
    /**
     * Get SpecialEffectManager instance
     */
     public static EndoscopeManager getInstance() {
         return Singleton.sINSTANCE;
     }
    
    /**
     * Singleton
     */
     private EndoscopeManager() {
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
      private class ServiceCallBackImp extends IEndoscopeCallback.Stub {
          
          @Override
          public void info(String msg) {
                long ident = Binder.clearCallingIdentity();
                try {
                    // notify UI
                    EndoscopeManager.this.mStatusListener.onInfo(msg);
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
          mBpEndoScopeService = IEndoscope.Stub.asInterface(
                             ServiceManager.getService(HpSystemServer.NAME.ENDOSCOPE));
          if (!checkServiceProxyValid()) {
              // notify UI
              if (listener != null) {
                  listener.onError("No binder proxy");
              }
              return;
          }
          
          // register callback
          try {
            mBpEndoScopeService.registerCallBack(mServiceCB);
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
                mBpEndoScopeService.unregisterCallBack(mServiceCB);
            } catch (RemoteException e) {
                HpLog.e(TAG, "RemoteException unregisterCallBack fail: " + e);
            }
      }
     
    
    /**
     * get endoscope id function
     */
    public int getEndoScopeId() {
        HpLog.i(TAG, "[getEndoScopeId] enter");
        int ret = -1;
        if (!checkServiceProxyValid()) {
            // notify UI
            if (this.mStatusListener != null) {
                this.mStatusListener.onError("No binder proxy");
            }
            return ret;
        }

        try {
            ret = mBpEndoScopeService.getEndoScopeId();
        } catch (RemoteException e) {
            HpLog.e(TAG, "RemoteException get endoscope id fail: " + e);
        }
        
        return ret;
    }
    
    
    /**
     * Binder proxy validity check
     */
    private boolean checkServiceProxyValid() {
        HpLog.i(TAG, "[checkServiceProxyValid] enter");
        return (mBpEndoScopeService == null)? false: true;
    }
    
}
