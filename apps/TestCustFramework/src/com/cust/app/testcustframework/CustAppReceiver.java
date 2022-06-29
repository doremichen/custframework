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

import custdroid.util.Print;

public class CustAppReceiver extends BroadcastReceiver {
    private static final String TAG = "HapticReceiver";
    
    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        Print.info(this, "action: " + action);
        if(action.equals(Intent.ACTION_BOOT_COMPLETED)) {
            Intent startIntent = new Intent();
            startIntent.setClass(context, CustBootService.class);
            context.startService(startIntent);
            
        }
    
    }

}
