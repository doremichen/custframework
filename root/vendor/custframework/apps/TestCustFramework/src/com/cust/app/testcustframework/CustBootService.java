/**
 * File: CustBootService.java
 * Breif: Used to invoke systen server to add cust spe service
 *
 * Name: Adam chen
 * Date: 2019/11/27
 */

package com.cust.app.testcustframework;

import android.app.Notification;
import android.app.Service;
import android.content.Intent;
import android.content.ContentResolver;
import android.os.Build;
import android.os.IBinder;
import android.provider.Settings;
import android.util.Log;

import com.cust.system.server.HpSystemServer;
import com.cust.system.server.HpSystemServer.ACTION;
//import custdroid.util.Print;

public class CustBootService extends Service {
    
    private static final String TAG = "CustBootService";
    
    //private static final int SERVICE_STARTED_NOTIFICATION_ID = Integer.MAX_VALUE;   // for android o
    
    @Override
    public IBinder onBind(Intent arg0) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.i(TAG, "[onCreate] enter");
        //Print.info(this, "[onCreate]");
        
        //if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
        //    startForeground(SERVICE_STARTED_NOTIFICATION_ID, new Notification());
        //}
       // start to add heliosPro service in system server
        HpSystemServer.init(this.getApplicationContext());
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.i(TAG, "[onDestroy] enter");
        //Print.info(this, "[onDestroy]");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i(TAG, "[onStartCommand] enter");
        //Print.info(this, "[onStartCommand]");
        // start to add cust service in system server
        if (intent != null) {
              String action = intent.getAction();
              if (ACTION.SWITCH_ADB_FUNCTION.equals(action)) {
                togleAdbConfig(getContentResolver());
              }
          }
        return START_STICKY;
    }


    private void togleAdbConfig(ContentResolver resolver) {
        if (Settings.Global.getInt(resolver, Settings.Global.ADB_ENABLED, 0) == 0) {
            // enable
            Settings.Global.putInt(resolver, Settings.Global.ADB_ENABLED, 1);
        } else {
            // disable
             Settings.Global.putInt(resolver, Settings.Global.ADB_ENABLED, 0);
        }
    }


}
