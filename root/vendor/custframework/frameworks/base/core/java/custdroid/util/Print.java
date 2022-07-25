/**
 * File: Print.java
 * Breif: provide to show log info
 *
 * Name: Adam chen
 * Date: 2019/11/28
 */
 
package custdroid.util;

import android.util.Log;

public final class Print {
    
    private Print() {}

    private static final String TAG = "CustFramework";

    public static void info(Object obj, String str) {
        Log.i(TAG, obj.getClass().getSimpleName() + ": " + str);
    }
    
    public static void error(Object obj, String str) {
        Log.e(TAG, obj.getClass().getSimpleName() + ": " + str);
    }
    
    public static void info(Class<?> clazz, String str) {
        Log.i(TAG, clazz.getSimpleName() + ": " + str);
    }
    
    public static void error(Class<?> clazz, String str) {
        Log.e(TAG, clazz.getSimpleName() + ": " + str);
    }
    
}
