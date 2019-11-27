/**
 *  Facade controller
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

import com.cust.server.SpecialEffectsService;

/**
 * Class that lets you access the Custdroid SpecialEffectsService.
 */
public final class SpecialEffectManager
{
    private static final String TAG = "SpecialEffectManager";
    private static final boolean DEBUG = true;
    
    
    //get Special effects service
    public static SpecialEffectsService getSpecialEffectsService(Context context) {
           PrintI("getSpecialEffectsService");
           
           SpecialEffectsService sef_svr =  new SpecialEffectsService(context);
           sef_svr.SpecialEffectInit();
           
           return sef_svr;
           
    }    
    
    private static void PrintI(String str) {
        if(DEBUG) Log.i(TAG, str);   
        
    }    
   
    
}
