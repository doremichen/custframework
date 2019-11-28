/**
 * File: CustSystemServer.java
 * Breif: Facade controller
 *
 * Name: Adam chen
 * Date: 2019/11/27
 */

package custdroid.hardware;

import android.content.Context;
import android.os.IBinder;
import android.os.ServiceManager;

import com.cust.server.SpecialEffectsService;
import custdroid.util.Print;

public final class CustSystemServer
{
    private static final String TAG = "CustSystemServer";
    
    private static class CustServerThread extends Thread {
    
        @Override
        public void run() {
            
            try {
                
                //java cust framework service
                Print.info(CustServerThread.class,"add SpecialEffectsService");
                ServiceManager.addService("cust.java.spe.sevice", new SpecialEffectsService());
            } catch (RuntimeException e) {
                
               Print.error(CustServerThread.class,"******************************************");
               Print.error(CustServerThread.class,"************ Failure starting core service" + e);
            }
        }    

    }
    
    /**
     * Start thread to add service in system server
     */
    public static final void init() {
        Print.info(CustSystemServer.class, "Entered the Cust system java framework server!");
             
        Thread thr = new CustServerThread();
        thr.setName("cust.system server.thread");
        thr.start();
    }
    
}
