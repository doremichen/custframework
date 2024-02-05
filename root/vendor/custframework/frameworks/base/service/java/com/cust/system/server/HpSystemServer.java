/**
 * File: HpSystemServer.java
 * Breif: Facade controller
 *
 * Name: 
 * Date:
 */

package com.cust.system.server;

import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.os.ServiceManager;

import com.cust.framework.server.dicom.DICOMManager;
import com.cust.framework.util.Print;
import com.cust.server.SpecialEffectsService;
import com.cust.server.phantom.PhantomService;
import com.cust.server.aidlservice.AIDLJavaService;

public final class HpSystemServer {
    private static final String TAG = HpSystemServer.class.getSimpleName();

    public interface ACTION {
        String SWITCH_ADB_FUNCTION = "action.switch.adb.function";
    } 

    public interface NAME {
        String CUSTService = "cust.java.spe.sevice";
        String PHANTOM = "phantom";
        String AIDLService = "cust.aidl.service";
    }

    private static Context sContext;

    // system service info
    public enum ServiceList {

        // Add service here
        SPE_SVR(NAME.CUSTService, new SpecialEffectsService()),
        PHANTOM_SVR(NAME.PHANTOM, new PhantomService(sContext)),
        AIDL_SVR(NAME.AIDLService, new AIDLJavaService(sContext));

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
                Print.i(TAG,"add service in service list!!!");
                // add service by service manager
                for (ServiceList element: ServiceList.values()) {
                    ServiceManager.addService(element.getSvrName(), element.getSvr());
                }
               
            } catch (RuntimeException e) {
                
               Print.e(TAG,"******************************************");
               Print.e(TAG,"************ Failure starting core service" + e);
            }
            
            Print.i(TAG,"system framewaork system services are ready...");
            
            Print.i(TAG,"start system framework core services....");
            for (HpSystemCoreServer coreSvr: sCoreServices) {
                String name = coreSvr.getClass().getSimpleName();
                Print.i(TAG,"start " + name);
                coreSvr.onStart();
            }

        }

    }
    
    /**
     * Start thread to add service in system server
     * @param context
     */
    public static final void init(Context context) {
        Print.i(TAG, "Entered the HeliosP system java framework server!");
        sContext = context;
        Thread thr = new initThread();
        thr.setName("heliosp.system server.thread");
        thr.start();
        
    }
    
    /**
     * send switch adb function request to heliosp system service
     * 
     */
    public static final void switchAdbFunc(Context context) {
        Print.i(TAG, "[swichAdbFunc] xxx");
        // intent
        Intent it = new Intent();
        it.setAction(ACTION.SWITCH_ADB_FUNCTION);
        it.setPackage("com.cust.app.hpservice");
        context.startService(it);
        Print.i(TAG, "[swichAdbFunc] +++");
    }
    
}
