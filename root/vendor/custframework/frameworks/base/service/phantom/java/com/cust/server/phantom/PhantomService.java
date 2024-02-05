/**
 * Copyright (C) xxxx
 */
package com.cust.server.phantom;

import android.app.ActivityManager;
import android.app.ActivityManagerNative;
import android.app.IActivityManager;
import android.app.Instrumentation;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Binder;
import android.os.LocaleList;
import android.os.PowerManager;
import android.os.RemoteException;
import android.os.StatFs;
import android.os.storage.DiskInfo;
import android.os.storage.VolumeInfo;
import android.os.SystemClock;
import android.os.UserHandle;
import android.view.MotionEvent;

import java.io.File;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import com.android.internal.app.LocalePicker;
import com.android.internal.app.LocaleStore;
import com.cust.framework.server.phantom.IPhantom;
import com.cust.framework.util.CommonUtils;
import com.cust.framework.util.CommonUtils.OnExecuteCallback;
import com.cust.framework.util.Print;


/**
 * HeliosPro PhantomService
 */
public final class PhantomService extends IPhantom.Stub {


    private static final String TAG = PhantomService.class.getSimpleName();

    private ExecutorService mWorkTask;
    private Instrumentation mInstrumentation;

    private Context mCtx;

    public PhantomService(Context context) {
       Print.i(TAG, "PhantomService startup");
       this.mCtx = context;
       mWorkTask = Executors.newFixedThreadPool(1);
       mInstrumentation = new Instrumentation();
    }
    
    /**
     * execute touch event
     * 
     * @param: x, y
     */
    @Override
    public void executeTouchEvent(float x, float y) {
       Print.i(TAG, "[executeTouchEvent]: x " + x + " y " + y);
       long ident = Binder.clearCallingIdentity();
       try {            
            workTask(x, y);
             
       } finally {
           Binder.restoreCallingIdentity(ident);
       }
    }
    
    /**
     * execute reboot
     * 
     */
    @Override
    public void executeReboot() {
       Print.i(TAG, "[executeReboot] enter");
       long ident = Binder.clearCallingIdentity();
       try {            
            PowerManager powerManager = (PowerManager) this.mCtx.getSystemService(Context.POWER_SERVICE);
            // reboot
            powerManager.reboot(null);
       } finally {
           Binder.restoreCallingIdentity(ident);
       }
       Print.i(TAG, "[executeReboot] exit");
    }
    
    /**
     * execute shutdown
     * 
     */
    @Override
    public void executeShutdown() {
       Print.i(TAG, "[executeShutdown] enter");
       long ident = Binder.clearCallingIdentity();
       try {            
            PowerManager powerManager = (PowerManager) this.mCtx.getSystemService(Context.POWER_SERVICE);
            // reboot
            powerManager.shutdown(false, null, false);
       } finally {
           Binder.restoreCallingIdentity(ident);
       }
       Print.i(TAG, "[executeShutdown] exit");
    }
    
    /**
     * master clear due to factory reset
     * 
     */
    @Override
    public void doMasterClear() {
        Print.i(TAG, "doMasterClear +++");
        Intent intent = new Intent(Intent.ACTION_FACTORY_RESET);
        intent.setPackage("android");
        intent.addFlags(Intent.FLAG_RECEIVER_FOREGROUND);
        intent.putExtra(Intent.EXTRA_REASON, "doMasterClear");
        intent.putExtra(Intent.EXTRA_WIPE_EXTERNAL_STORAGE, false);
        intent.putExtra(Intent.EXTRA_WIPE_ESIMS, false);
        mCtx.sendBroadcast(intent);
        Print.i(TAG, "doMasterClear xxx");
    }

    private final List<LocaleStore.LocaleInfo> mFeedItemList = new ArrayList<>();;

    /**
     * update locale
     * 
     */
    @Override
    public void addLocale(String localeId) {
        Print.i(TAG, "addLocale ++++: " + localeId);
        // get Locale
        String[] localArray = localeId.split("_");
        String langCode = localArray[0];
        String countryCode = (localArray.length == 2)? localArray[1]: "";
        Locale locale = new Locale(langCode, countryCode);
        //Locale locale = Locale.forLanguageTag(localeId);
        try {
            IActivityManager am = ActivityManager.getService();
            Configuration config = am.getConfiguration();
            config.setLocale(locale);
            config.userSetLocale = true;

            am.updatePersistentConfiguration(config);
        } catch (RemoteException e) {
            e.printStackTrace();
        }

//        try {
//            LocaleStore.LocaleInfo li = newLocaleInfo(localeId);
//            mFeedItemList.add(li);
//            doTheUpdate();
//        } catch (NoSuchMethodException e) {
//            e.printStackTrace();
//        } catch (InvocationTargetException e) {
//            e.printStackTrace();
//        } catch (InstantiationException e) {
//            e.printStackTrace();
//        } catch (IllegalAccessException e) {
//            e.printStackTrace();
//        }

        Print.i(TAG, "addLocale xxxx");
    }

    @Override
    public void startStorageWizardFormatProgress(DiskInfo diskinfo) {
        Print.i(TAG, "[startStorageWizardFormatProgress] +++ ");
        Intent intent  = new Intent();
         intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//         intent.setClassName("com.android.settings",
//                 "com.android.settings.deviceinfo.StorageWizardInit");
         intent.setClassName("com.android.settings",
                 "com.android.settings.deviceinfo.StorageWizardFormatProgress");
//         intent.putExtra(VolumeInfo.EXTRA_VOLUME_ID, vol.getId());
         intent.putExtra(DiskInfo.EXTRA_DISK_ID, diskinfo.getId());
         intent.putExtra("format_forget_uuid", (String) null);
         intent.putExtra("format_private", false);
         Print.i(TAG, "[onVolumeStateChanged] start activity");
         this.mCtx.startActivity(intent);
        Print.i(TAG, "[startStorageWizardFormatProgress] xxx ");
    }
     
     ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     public interface UsbStorageKey {
         String TOTAL = "usb.storage.total.key";
          String AVAILABLE = "usb.storage.available.key";
          String FREE = "usb.storage.free.key";
     }
     
     public static final String ACTION_GET_USB_SPACE = "com.cust.intent.action.USB_SPACE_INFO";
     
     @Override
    public void getSizeByStatFs(VolumeInfo info) {
        Print.i(TAG, "[getSizeByStatFs] +++ ");
        Print.i(TAG, "volume info: " + info.toString());
        File filePath = info.getPath();
        if (filePath == null) {
            Print.i(TAG, "[getSizeByStatFs] path not found!!!");
            sendAction(0L, 0L, 0L);
            return;
        }
        
        StatFs statFs = new StatFs(filePath.getPath());
        Print.i(TAG, "[getSizeByStatFs] path: " + filePath.getPath());
        long blockSize = statFs.getBlockSizeLong();
        long totalSize = statFs.getBlockCountLong()*blockSize;
        Print.i(TAG, "[getSizeByStatFs] totalSize: " + totalSize);
        long availableSize = statFs.getAvailableBlocksLong()*blockSize;
        Print.i(TAG, "[getSizeByStatFs] availableSize: " + availableSize);
        long freeSize = statFs.getFreeBlocksLong()*blockSize;
        Print.i(TAG, "[getSizeByStatFs] freeSize: " + freeSize);
        
        // Send broadcast to app
        sendAction(totalSize, availableSize, freeSize);
        //Intent intentUsbSpace = new Intent(ACTION_GET_USB_SPACE);
        //intentUsbSpace.addFlags(Intent.FLAG_RECEIVER_REPLACE_PENDING
        //    | Intent.FLAG_RECEIVER_INCLUDE_BACKGROUND
        //    | Intent.FLAG_RECEIVER_FOREGROUND);
        // //Intent data
        // intentUsbSpace.putExtra(UsbStorageKey.TOTAL, totalSize);
        // intentUsbSpace.putExtra(UsbStorageKey.AVAILABLE, availableSize);
        // intentUsbSpace.putExtra(UsbStorageKey.FREE, freeSize);
        //mCtx.sendBroadcastAsUser(intentUsbSpace, UserHandle.ALL);
        Print.i(TAG, "[getSizeByStatFs] xxx ");
    }
    
    @Override
    public void sendStorageEmptyInfo() {
        Print.i(TAG, "[sendStorageEmptyInfo] +++ ");
        sendAction(0L, 0L, 0L);
        Print.i(TAG, "[sendStorageEmptyInfo] xxx ");
    }
    
    
    private void sendAction(long totalSize, long availableSize, long freeSize) {
        Print.i(TAG, "[sendAction] +++ ");
        Intent intentUsbSpace = new Intent(ACTION_GET_USB_SPACE);
        intentUsbSpace.addFlags(Intent.FLAG_RECEIVER_REPLACE_PENDING
            | Intent.FLAG_RECEIVER_INCLUDE_BACKGROUND
            | Intent.FLAG_RECEIVER_FOREGROUND);
         //Intent data
         intentUsbSpace.putExtra(UsbStorageKey.TOTAL, totalSize);
         intentUsbSpace.putExtra(UsbStorageKey.AVAILABLE, availableSize);
         intentUsbSpace.putExtra(UsbStorageKey.FREE, freeSize);
        mCtx.sendBroadcastAsUser(intentUsbSpace, UserHandle.ALL);
        Print.i(TAG, "[sendAction] xxx ");
    }
    
     ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    interface DialogInformation {
        String USB = "UsbPermissionActivity";
        String BOOT = "GrantPermissionsActivity";
    }
    
    enum PermissionDialog {
        USB(DialogInformation.USB,
            "1400",
            "650"),
        BOOT(DialogInformation.BOOT,
            "960",
            "810");
        
        private String mName, mX, mY;
        private PermissionDialog(String name, String x, String y) {
            this.mName = name;
            this.mX = x;
            this.mY = y;
        }
        
        public String getName() {
            return this.mName;
        }
        
        public String getX() {
            return this.mX;
        }
        
        public String getY() {
            return this.mY;
        }
        
        public static PermissionDialog get(String info) {
            for (PermissionDialog dialog: PermissionDialog.values()) {
                Print.i(TAG, "dialog name: " + dialog.mName);
                if (info.contains(dialog.mName)) {
                    return dialog;
                }
            }
            return null;
        }
    }
    
    private Runnable mSendTouchEventTask = new Runnable() {
                @Override
                public void run() {
                        Print.i(TAG, "SendTouchEventTask run...");
                        
                        // test
                        // dumpsys activity activities | grep mResumedActivity
                        CommonUtils.execute(new OnExecuteCallback() {
                                                @Override
                                                public void onResult(String msg) {
                                                    Print.i(TAG, "onResult: " + msg);
                                                    PermissionDialog dialog = PermissionDialog.get(msg);
                                                    if (dialog != null) {
                                                        Print.i(TAG, "PermissionDialog: " + dialog.getName());
                                                        CommonUtils.execute(null, "input", "tap", dialog.getX(), dialog.getY());
                                                    }
                                                }
                        }, "sh", "-c", "dumpsys activity activities | grep mResumedActivity");
                        
                        // TODO: test
                        //CommonUtils.execute("input", "tap", "475", "100");
                        //Phantom phantom = Phantom.getInstance();
                        //phantom.executeTouchEvent(478.4398f, 100.0f);
                        //Print.i(TAG, "task is complete!!!");
                    }
     };
    

    /**
     * 
     */
    @Override
    public void sendTouchEvent(boolean start) {
        if (start == true) {
            CommonUtils.executeWithSchedule(mSendTouchEventTask, 1000L, TAG);
            //CommonUtils.delayOf(mSendTouchEventTask, 500L);
        } else {
            CommonUtils.cancelSchedule(TAG);
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    private LocaleList mLocalesToSetNext = null;
    private LocaleList mLocalesSetLast = null;


    public void doTheUpdate() {
        Print.i(TAG, "doTheUpdate +++");
        int count = mFeedItemList.size();
        final Locale[] newList = new Locale[count];

        for (int i = 0; i < count; i++) {
            final LocaleStore.LocaleInfo li = mFeedItemList.get(i);
            newList[i] = li.getLocale();
        }

        final LocaleList ll = new LocaleList(newList);
        updateLocales(ll);
        Print.i(TAG, "doTheUpdate xxx");
    }


    private void updateLocales(final LocaleList localeList) {
        Print.i(TAG, "updateLocales +++");
        // pre-check
        if (localeList.equals(mLocalesToSetNext)) {
            Print.e(TAG, "has been updated!!!");
            return;
        }

        // This will only update the Settings application to make things feel more responsive,
        // the system will be updated later, when animation stopped.
        LocaleList.setDefault(localeList);

        mLocalesToSetNext = localeList;

        LocalePicker.updateLocales(mLocalesToSetNext);
        mLocalesSetLast = mLocalesToSetNext;

        mLocalesToSetNext = null;
        Print.i(TAG, "updateLocales xxx");
    }

    private LocaleStore.LocaleInfo newLocaleInfo(String arg) throws NoSuchMethodException,
            InvocationTargetException,
            InstantiationException,
            IllegalAccessException {
        Print.i(TAG, "newLocaleInfo +++: " + arg);
        Class<LocaleStore.LocaleInfo> clazz = LocaleStore.LocaleInfo.class;
        Constructor<?> constructor = clazz.getDeclaredConstructor(String.class);
        constructor.setAccessible(true);
        Print.i(TAG, "newLocaleInfo xxx");
        return (LocaleStore.LocaleInfo) constructor.newInstance(arg);
    }

    
    private void workTask(float x, float y) {
        Print.i(TAG, "workTask action");
        mWorkTask.execute(new Runnable() {
                @Override
                public void run() {
                    monitorTouch(x, y);
                }
            });
    }
    
    private void monitorTouch(float x, float y) {
        Print.i(TAG, "monitorTouch action");
        synchronized(PhantomService.class) {
            Print.i(TAG, "start monitor touch down");
            mInstrumentation.sendPointerSync(MotionEvent.obtain(
                           SystemClock.uptimeMillis(),
                           SystemClock.uptimeMillis(),
                           MotionEvent.ACTION_DOWN, x, y, 0));
            Print.i(TAG, "start monitor touch up");
            mInstrumentation.sendPointerSync(MotionEvent.obtain(
                           SystemClock.uptimeMillis(),
                           SystemClock.uptimeMillis(),
                           MotionEvent.ACTION_UP, x, y, 0));
        }
        
    }
    
}
