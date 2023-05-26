/**
 * File: HpSystemServer.java
 * Breif: Facade controller
 *
 * Name: 
 * Date:
 */

package com.med.hp.server;

import android.content.Context;
import android.os.IBinder;
import android.os.ServiceManager;

import com.med.hpframework.server.dicom.DICOMManager;
import com.med.server.endoscope.EndoScopeService;
import com.med.server.led.LedService;
import com.med.hpframework.util.HpLog;

public final class HpSystemServer {
    private static final String TAG = HpSystemServer.class.getSimpleName();

    public interface NAME {
        String ENDOSCOPE = "endoscopeservice";
        String LED = "ledservice";
    }

    private static Context sContext;

    // system service info
    public enum ServiceList {

        // Add service here
        //SPE_SVR("cust.java.spe.sevice", new SpecialEffectsService());
        ENDOSCOPE_SVR(NAME.ENDOSCOPE, new EndoScopeService(sContext)),
        LED_SVR(NAME.LED, new LedService(sContext));

        private String mSvrName;
        private IBinder mSvr;

        private ServiceList(String tag, IBinder svr) {
            this.mSvrName = tag;
            this.mSvr = svr;
        }

        public String getSvrName() {
            return this.mSvrName;
        }

        public IBinder getSvr() {
            return this.mSvr;
        }
    }
    
    // core service info
    private static HpSystemCoreServer[] sCoreServices = {
        
        new DICOMManager(),
        
    };
    
    
    /**
     * register HeliosP services and execute POST
     */
    private static class initThread extends Thread {

        public initThread() {}

        @Override
        public void run() {
            
            try {
                HpLog.i(TAG,"add service in service list!!!");
                // add service by service manager
                for (ServiceList element: ServiceList.values()) {
                    ServiceManager.addService(element.getSvrName(), element.getSvr());
                }
               
            } catch (RuntimeException e) {
                
               HpLog.e(TAG,"******************************************");
               HpLog.e(TAG,"************ Failure starting core service" + e);
            }
            
            HpLog.i(TAG,"hp framewaork system services are ready...");
            
            HpLog.i(TAG,"start hp framework core services....");
            for (HpSystemCoreServer coreSvr: sCoreServices) {
                String name = coreSvr.getClass().getSimpleName();
                HpLog.i(TAG,"start " + name);
                coreSvr.onStart();
            }

        }

    }
    
    /**
     * Start thread to add service in system server
     * @param context
     */
    public static final void init(Context context) {
        HpLog.i(TAG, "Entered the HeliosP system java framework server!");
        sContext = context;
        Thread thr = new initThread();
        thr.setName("heliosp.system server.thread");
        thr.start();
        
    }
    
}
