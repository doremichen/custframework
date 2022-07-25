/**
 * File: CustAppReceiver.java
 * Breif: Recieve boot compeleted and then trigger cust boot service
 *
 * Name: Adam chen
 * Date: 2019/11/27
 */

package com.cust.app.testcustframework;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.UserHandle;
import android.util.Log;

//import custdroid.util.Print;

public class CustAppReceiver extends BroadcastReceiver {
    private static final String TAG = "CustAppReceiver";
    
    @Override
    public void onReceive(Context context, Intent intent) {
        Log.i(TAG, "[onReceive] enter");
        //Print.info(this, "[onReceive] enter");
        String action = intent.getAction();
        Log.i(TAG, "action: " + action);
        //Print.info(this, "action: " + action);
        if(action.equals(Intent.ACTION_BOOT_COMPLETED)) {
            context.startServiceAsUser(new Intent(context, CustBootService.class), UserHandle.SYSTEM);
        }
    
    }

}
