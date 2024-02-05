/**
 * Copyright (C) 2023 The Altek medical Project
 */
package com.cust.framework.util;

import android.app.AlarmManager;
import android.content.Context;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.os.PowerManager;
import android.os.SystemProperties;
import android.provider.Settings;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class CommonUtils {

    // control the log function.
    public static final boolean IS_LOG = true;
    public static final String PERSIST_VENDOR_MED_SYSTEM_SHUTDOWN = "persist.vendor.med.system.shutdown";
    
    public interface ACTION_ENDOSCOPE {
        String PLUG = "com.med.action.endoscope.plug";
        String KEY_STATE = "key.plug.state";
    }
    

    private static final String TAG = CommonUtils.class.getSimpleName();
    private static final String TAG_SCHEDULE = ScheduleUtils.class.getSimpleName();

    private static final int CMD_READ_BUFFER_SIZE = 8 * 1024;
    
    private static ScheduledExecutorService sScheduleSvr;
    private  static ScheduledFuture<?> sScheduleFuture;

    public static boolean sUpdateLanguage;
    
    static {
        sScheduleSvr = Executors.newScheduledThreadPool(1);
    }

    public interface FileFormat {
        String BASIC = "basic";
        String DICOM = "dicom";
    }

    public interface ImageFileType {
        String IMAGE = "image/jpeg";
        String VIDEO = "video/mp4";
    }

    public interface PowerButtonProperty {
        int ON = 1;
        int OFF = 0;
    }

    public interface TimeFormat {
        String FORMAT_12 = "12";
        String FORMAT_24 = "24";
    }

    public interface RangeType {
        int ALL = 0;
        int ONE_DAY = 1;
        int ONE_WEEK = 2;
        int ONE_MONTH = 3;
        int SIX_MONTH = 4;
        int ONE_YEAR = 5;
    }

    public static void info(String tag, String msg) {
        if (IS_LOG) Log.i(tag, msg);
    }

    public static void error(String tag, String msg) {
        if (IS_LOG) Log.e(tag, msg);
    }

    public static void warning(String tag, String msg) {
        if (IS_LOG) Log.w(tag, msg);
    }
    
    public static String formatDateAndTime(long timestamp) {
        StringBuilder sb = new StringBuilder();

        Calendar cal = Calendar.getInstance();
        cal.setTimeInMillis(timestamp);
        sb.append(cal.get(Calendar.YEAR));
        sb.append("/");
        sb.append(cal.get(Calendar.MONTH) + 1); // month value: 0 ~ 11
        sb.append("/");
        sb.append(cal.get(Calendar.DAY_OF_MONTH));
        sb.append(" ");
        sb.append(String.format("%02d", cal.get(Calendar.HOUR_OF_DAY))); // TODO: consider 12 hour format
        sb.append(":");
        sb.append(String.format("%02d", cal.get(Calendar.MINUTE)));

        return sb.toString();
    }

    public static String formatTime(long timestamp, boolean is24h) {
        SimpleDateFormat formatter = null;
        Calendar cal = Calendar.getInstance();
        cal.setTimeInMillis(timestamp);
        if (is24h) {
            formatter = new SimpleDateFormat("HH:mm");
        } else {
            formatter = new SimpleDateFormat("hh:mm a", Locale.getDefault());
        }

        return formatter.format(cal.getTime());
    }

    public static String formatDate(long timestamp, String[] monthMap) {
        Calendar cal = Calendar.getInstance();
        cal.setTimeInMillis(timestamp);
        return String.format("%04d/%02d/%s", cal.get(Calendar.DAY_OF_MONTH), monthMap[cal.get(Calendar.MONTH)]);
    }

    public static int convertDpToPx(int dp){
        DisplayMetrics metrics = Resources.getSystem().getDisplayMetrics();
        int px = Math.round(dp*(metrics.densityDpi/DisplayMetrics.DENSITY_DEFAULT));
        info(TAG, "[convertDpToPx] input: " + dp + ", densityDpi: " + metrics.densityDpi + ", return: " + px + ", density: " + metrics.density);
        return px;

    }

    /**
     * Print call stack information
     * @param tag
     */
    public static void printCallStack(String tag) { 
        StackTraceElement[] elements = Thread.currentThread().getStackTrace();
        info(tag, "= call stack start ===================================");
        for (StackTraceElement element : elements) {
            CommonUtils.info(tag, element.toString());
        }
        info(tag, "= call stack end =====================================");
    }

    /**
     * hide system bar and make it full screen
     * @param window activity window
     */
    public static void hideSystemUI(Window window) {
        View decorView = window.getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                        | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        // Set the content to appear under the system bars so that the
                        // content doesn't resize when the system bars hide and show.
                        | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        // Hide the nav bar and status bar
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN);

        window.addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

    }

    /**
     * Set start time is 0L if the task would be executed immediately
     *
     * @param task  the specified action
     * @param start the specified period
     */
    public static void execute(Runnable task, long start) {
         // start
         sScheduleSvr.schedule(task, start, TimeUnit.MILLISECONDS);
    }


    /**
     * Schedule tool
     */
    private static class ScheduleUtils {
        private ScheduledExecutorService mScheduleSvr;
        private  ScheduledFuture<?> mScheduleFuture;

        public ScheduleUtils() {
            mScheduleSvr = Executors.newScheduledThreadPool(1);
        }

        @Override
        protected void finalize() throws Throwable {
            super.finalize();
            try {
                mScheduleSvr.shutdown();
                boolean dumpDone = mScheduleSvr.awaitTermination(5, TimeUnit.SECONDS);
                if (!dumpDone) {
                    error(TAG_SCHEDULE, "Failed to complete,"
                            + " dumping what we have so far.");
                }
            } catch (InterruptedException e) {
                // Ignore
            }
        }

        /**
         * start schedule according to the specified duration time
         * @param task work task
         * @param delay duration time
         */
        public void start(Runnable task, long delay) {
            info(TAG_SCHEDULE, "start:");
            // schedule task check
            if (mScheduleFuture != null) {
                // cancel
                mScheduleFuture.cancel(false);
            }

            // schedule task
            mScheduleFuture = mScheduleSvr.scheduleAtFixedRate(task, delay, delay, TimeUnit.MILLISECONDS);

        }

        /**
         * stop schedule task
         */
        public void stop() {
            info(TAG_SCHEDULE, "stop:");
            // schedule task check
            if (mScheduleFuture != null) {
                // cancel
                mScheduleFuture.cancel(false);
                mScheduleFuture = null;
            }
        }

    }

    // Schedule task map
    private static Map<String, ScheduleUtils> sScheduleMap = new HashMap<>();

    public static void executeWithSchedule(Runnable task, long delay, String tag) {
        info(TAG_SCHEDULE, "executeWithSchedule");
        // get from schedule map
        ScheduleUtils scheduleTool = sScheduleMap.get(tag);
        // put in map if no task in map
        if (scheduleTool == null) {
            scheduleTool = new ScheduleUtils();
            sScheduleMap.put(tag, scheduleTool);
        }

        // execute
        scheduleTool.start(task, delay);
    }

    public static void cancelSchedule(String tag) {
        info(TAG_SCHEDULE, "cancelSchedule");
        // get from schedule map
        ScheduleUtils scheduleTool = sScheduleMap.get(tag);
        // no action because no task in map
        if (scheduleTool == null) {
            error(TAG_SCHEDULE, "No task in schedule map!!!");
            throw new RuntimeException("No task in schedule map!!!");
        }

        // execute
        scheduleTool.stop();
    }

    /**
     * Excute task after subsequently with the given period
     *
     * @param task  : the specified action
     * @param delay : the specified period
     */
    @Deprecated
    public static void executeWithSchedule(Runnable task, long delay) {
        info(TAG, "executeWithSchedule:");
        // schedule task check
        if (sScheduleFuture != null) {
            // cancel
            sScheduleFuture.cancel(false);
        }

        // schedule task
        sScheduleFuture = sScheduleSvr.scheduleAtFixedRate(task, delay, delay, TimeUnit.MILLISECONDS);

    }
    
    /**
     * Cancel the specified action
     */
    @Deprecated
    public static void cancelSchedule() {
        info(TAG, "cancelSchedule:");
        // schedule task check
        if (sScheduleFuture != null) {
            // cancel
            sScheduleFuture.cancel(false);
            sScheduleFuture = null;
        }
    }
    
    public interface OnExecuteCallback {
        
        void onResult(String msg);
    }
    
    
    /**
     * example: CommonUtils.execute("ls", "-al");
     */
    public static void execute(OnExecuteCallback callback, String... cmd) {
        info(TAG, "execute:");
        dumpInfo(cmd);
        try {
            ProcessBuilder processBuilder = new ProcessBuilder().redirectErrorStream(true);
            processBuilder.command(cmd);

            // start
            Process process = processBuilder.start();
            
            BufferedReader reader =
                    new BufferedReader(new InputStreamReader(process.getInputStream()), CMD_READ_BUFFER_SIZE);

            StringBuilder sb = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                info(TAG, line);
                sb.append(line);
            }
            
            // callback
            if (callback != null) {
                callback.onResult(sb.toString());
            }
            

            int exitCode = process.waitFor();
            info(TAG, "\nExited with error code : " + exitCode);
          } catch (IOException e) {
              e.printStackTrace();
          } catch (InterruptedException e) {
              e.printStackTrace();
          }
       info(TAG, "execute finish...");
    }

    /**
     * set shut down property for long press power button when exporting
     * set wakeup lock for short press power button when exporting
     * @param property  refer to PowerButtonProperty
     */
    public static void setPowerButtonProperty(int property) {
        CommonUtils.info(TAG, "[setPowerButtonProperty] +++++++++++property = " + property);

        switch (property) {
            case CommonUtils.PowerButtonProperty.ON:
                // Note: enable in platform, android phone is not working
                SystemProperties.set(PERSIST_VENDOR_MED_SYSTEM_SHUTDOWN, String.valueOf(PowerButtonProperty.ON));
                break;
            case CommonUtils.PowerButtonProperty.OFF:
                // Note: enable in platform, android phone is not working
                SystemProperties.set(PERSIST_VENDOR_MED_SYSTEM_SHUTDOWN, String.valueOf(PowerButtonProperty.OFF));
                break;
            default:
                break;
        }

        CommonUtils.info(TAG, "[setPowerButtonProperty] -----------property = " + property);
    }

    private static final String TAG_PowerWakeLock = PowerWakeLock.class.getSimpleName();
    private static final long WAKE_LOCK_TIME = 10*60*1000L;
    public static class PowerWakeLock {

        private PowerManager.WakeLock mWakeLock;

        public PowerWakeLock(Context context) {
            PowerManager powermanager = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
            mWakeLock = powermanager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK
                    , TAG_PowerWakeLock);
        }

        public void enable(boolean enabled) {
            if (enabled) {
                mWakeLock.acquire(WAKE_LOCK_TIME);
            } else {
                if (mWakeLock.isHeld()) {
                    mWakeLock.release();
                }
            }
        }

    }

    /**
     * Null object check. If the given object is valid, then return true. Otherwise, it shall return
     * false.
     *
     * @param obj
     * @return
     */
    public static boolean nullCheck(Object obj) {
        return obj != null;
    }

    
    /**
     * dump the string array information.
     * @param info: the string array data
     */
    private static void dumpInfo(String... info) {
        info(TAG, "================================");
        StringBuffer stb = new StringBuffer();
        for (String data: info) {
            stb.append(data).append(" ");
        }
        info(TAG, "info: " + stb.toString());
        info(TAG, "================================");
    }
}
