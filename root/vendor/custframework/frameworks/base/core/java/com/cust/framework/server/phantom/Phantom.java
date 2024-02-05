/**
 * File: Phantom.java
 * Breif: Led API
 *
 * Name: 
 * Date: 
 */

package com.cust.framework.server.phantom;

import android.os.Binder;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.storage.DiskInfo;
import android.os.storage.VolumeInfo;

import com.cust.system.server.HpSystemServer;
import com.cust.framework.util.Print;
import com.cust.framework.server.phantom.IPhantom;

/**
 * Class that lets you access the EndoScope service.
 */
public final class Phantom
{
    
    private static final String TAG = Phantom.class.getSimpleName();
    
    // Binder proxy
    private IPhantom mBpPhantomService;
    
    private static class Singleton {
        private static final Phantom sINSTANCE = new Phantom();
    }
    
    /**
     * Get SpecialEffectManager instance
     */
     public static Phantom getInstance() {
         return Singleton.sINSTANCE;
     }
    
    /**
     * Singleton
     */
     private Phantom() {
        mBpPhantomService = IPhantom.Stub.asInterface(
                             ServiceManager.getService(HpSystemServer.NAME.PHANTOM));
        if (!checkServiceProxyValid()) {
            Print.i(TAG, "phantom service proxy is null!!!");
            return;
        }
     }

     
     /**
      * execute touch event
      */
      public void executeTouchEvent(float x, float y) {
          Print.i(TAG, "[executeTouchEvent] enter");
          
          try {
                mBpPhantomService.executeTouchEvent(x, y);
          } catch (RemoteException e) {
              Print.e(TAG, "RemoteException execute touch event fail: " + e);
          }
      }
      
      /**
      * execute device reboot
      */
      public void executeReboot() {
          Print.i(TAG, "[executeReboot] enter");
          
          try {
                mBpPhantomService.executeReboot();
          } catch (RemoteException e) {
              Print.e(TAG, "RemoteException execute reboot fail: " + e);
          }
      }
      
      /**
      * execute device shutdown
      */
      public void executeShutdown() {
          Print.i(TAG, "[executeShutdown] enter");
          
          try {
                mBpPhantomService.executeShutdown();
          } catch (RemoteException e) {
              Print.e(TAG, "RemoteException execute shutdown fail: " + e);
          }
      }
      
      /**
      * execute touch event
      */
      public void doMasterClear() {
        Print.i(TAG, "[doMasterClear] enter");
        try {
            mBpPhantomService.doMasterClear();
        } catch (RemoteException e) {
            Print.e(TAG, "RemoteException execute reboot fail: " + e);
        }
      }

    /**
     * update locale
     */
      public void addLocale(String localeId) {
          Print.i(TAG, "[addLocale] enter");
          try {
              mBpPhantomService.addLocale(localeId);
          } catch (RemoteException e) {
              Print.e(TAG, "RemoteException execute reboot fail: " + e);
          }
      }
      
      
      public void startStorageWizardFormatProgress(DiskInfo diskinfo) {
           Print.i(TAG, "[startStorageWizardFormatProgress] enter");
          try {
              mBpPhantomService.startStorageWizardFormatProgress(diskinfo);
          } catch (RemoteException e) {
              Print.e(TAG, "RemoteException execute startStorageWizardFormatProgress fail: " + e);
          }
      }
      
    /**
     * get the space of usb storage thru phantom service
     */
      public void getSizeByStatFs(VolumeInfo info) {
          Print.i(TAG, "[getSizeByStatFs] enter");
          try {
              mBpPhantomService.getSizeByStatFs(info);
          } catch (RemoteException e) {
              Print.e(TAG, "RemoteException execute getSizeByStatFs fail: " + e);
          }
      }
      
     /**
     * send empty info of usb storage to app thru phantom service
     */
      public void sendStorageEmptyInfo() {
          Print.i(TAG, "[sendStorageEmptyInfo] enter");
          try {
              mBpPhantomService.sendStorageEmptyInfo();
          } catch (RemoteException e) {
              Print.e(TAG, "RemoteException execute sendStorageEmptyInfo fail: " + e);
          }
      }
      
    /**
     * execute send touch event 
    */
    public void sendTouchEvent(boolean start) {
        try {
              mBpPhantomService.sendTouchEvent(start);
        } catch (RemoteException e) {
              Print.e(TAG, "RemoteException execute reboot fail: " + e);
        }
    }
      
    /**
     * Binder proxy validity check
     */
    private boolean checkServiceProxyValid() {
        Print.i(TAG, "[checkServiceProxyValid] enter");
        return (mBpPhantomService == null)? false: true;
    }
    
    
    
}
