/**
 * File: CustBootService.java
 * Breif: Used to invoke systen server to add cust spe service
 *
 * Name: Adam chen
 * Date: 2019/11/27
 */

package com.cust.app.testcustframework;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

import custdroid.hardware.CustSystemServer;
import custdroid.util.Print;

public class CustBootService extends Service {
    
    @Override
    public IBinder onBind(Intent arg0) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Print.info(this, "[onCreate]");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Print.info(this, "[onDestroy]");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Print.info(this, "[onStartCommand]");
        // start to add cust service in system server
        CustSystemServer.init();
        return super.onStartCommand(intent, flags, startId);
    }
}
