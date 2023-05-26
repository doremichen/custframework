/**
 * File: HpLog.java
 * Breif: provide to show log info
 *
 * Name: 
 * Date:
 */
 
package com.med.hpframework.util;

import android.util.Log;

public final class HpLog {
    
    private HpLog() {}

    private static final String TAG = "HeliosPro_";

    private static final boolean LOG=true;
    
    public static void v(String tag, String msg) {
        if (LOG)  Log.v(TAG + tag, msg);
    }
    
    public static void d(String tag, String msg) {
        if (LOG)  Log.d(TAG + tag, msg);
    }
    
    public static void w(String tag, String msg) {
        if (LOG)  Log.w(TAG + tag, msg);
    }
    
    public static void i(String tag, String msg) {
        if (LOG)  Log.i(TAG + tag, msg);
    }

    public static void e(String tag, String msg) {
        if (LOG)  Log.e(TAG + tag, msg);
    }
    
    public static void getStackTrace(Throwable tr) {
        if (LOG)  Log.getStackTraceString(tr);
    }
}
