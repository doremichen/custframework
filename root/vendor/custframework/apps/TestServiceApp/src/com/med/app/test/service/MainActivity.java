/**
 * File: MainActivity.java
 * Breif: Test Service function
 *
 * Name: 
 * Date: 
 */

package com.med.app.test.service;

import android.app.AlertDialog;
import android.app.ListActivity;
import android.content.DialogInterface;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemProperties;
import android.provider.Settings;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.Toast;

import java.io.IOException;
import java.lang.StringBuilder;
import java.util.ArrayList;
import java.util.List;

import junit.framework.Assert;

import com.med.server.serialport.SerialPort;
import com.med.server.adc.ButtonDeviceManager;
import com.med.server.adc.ButtonDeviceManager.onADCButtonListener;

import com.med.hpframework.hardware.endoscope.EndoscopeManager;
import com.med.hpframework.hardware.led.LedManager;


import com.med.hpframework.util.HpLog;


public class MainActivity extends ListActivity {
    
    private static final String TAG = MainActivity.class.getSimpleName();
    
    private static Context CTX;
    
    private ListAdapter mAdapter;
    
    private static final int SHOW_TOAST = 1;
    private static final String KEY_INFO = "key.info";
    
    private static Handler sHandler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what){
                case SHOW_TOAST:
                    Bundle data = msg.getData();
                    String info = data.getString(KEY_INFO);
                    Toast.makeText(CTX, info, Toast.LENGTH_SHORT).show();
                    break;
                default:
                    break;
            }
        }
    };
    
    
    /**
     * List item info
     */
    private enum ItemContext {
        testADCButton("test adc button event") {
            
            @Override
            public void process(Context ctx) {
                HpLog.i(TAG_ITEM, "[process] enter");
                ButtonDeviceManager button = ButtonDeviceManager.newInstance();
                // set listener
                button.setOnADCButtonListner(new onADCButtonListener() {
                    @Override
                    public void onState(String button_event) {
                        HpLog.i(TAG_ITEM, "adc button event: " + button_event);
                    }
                });
                // start to detect adc button
                button.init();
                
                showToast("test adc button start!!!");
            }
            
            @Override
            public String toString() {
                return mName;
            }
        },
        testUart("test serial port") {
            
            @Override
            public void process(Context ctx) {
                HpLog.i(TAG_ITEM, "[process] enter");
                final SerialPort uart = new SerialPort();
                uart.openPort();
                // delay to close
                new Handler().postDelayed( new Runnable(){
                    @Override
                    public void run() {
                        // close usrt
                        uart.closePort();
                        showToast("uart close success!!!");
                    }}, 10000L);
                showToast("uart open success!!!");
                
                // write command
                final byte[] test_datas = new byte[] {
                    (byte)0xe0, 0x4f, (byte)0xd0,
                    0x20, (byte)0xea, 0x3a, 0x69, 0x10, (byte)0xa2, (byte)0xd8, 0x08, 0x00, 0x2b,
                    0x30, 0x30, (byte)0x9d   
                };
                // test sendCommand flow
                uart.writeCommand(test_datas);
                    
                
            }
            
            @Override
            public String toString() {
                return mName;
            }
        },
        testEnableAdb("test enable/disable adb") {
            
            @Override
            public void process(Context ctx) {
                HpLog.i(TAG_ITEM, "[process] enter");
                if (Settings.Global.getInt(ctx.getContentResolver(), Settings.Global.ADB_ENABLED, 0) == 0) {
                    // enable
                    Settings.Global.putInt(ctx.getContentResolver(), Settings.Global.ADB_ENABLED, 1);
                    showToast("enable adb!!!");
                } else {
                    // disable
                     Settings.Global.putInt(ctx.getContentResolver(), Settings.Global.ADB_ENABLED, 0);
                    showToast("disable adb!!!");
                }
            }
            
            @Override
            public String toString() {
                return mName;
            }
        },
        testFoo_LedManager("test foo@LedManager") {
            
            private LedManager.OnStatusListener mListener = new LedManager.OnStatusListener() {
                /**
                 * As following methods are invoked from LedManager controller
                 */
                @Override
                public void onError(String str) {
                    showToast(str);
                }
                
                @Override
                public void onInfo(String str) {
                    showToast(str);
                }
            };
            
            @Override
            public void process(Context ctx) {
                HpLog.i(TAG_ITEM, "[process] enter");
                if (ctx == null) {
                    throw new IllegalArgumentException("No context!!!");
                }
                
                LedManager mgr = LedManager.getInstance();
                
                mgr.init(mListener);
                int id = mgr.foo();
                Assert.assertEquals(0, id);
                // show result
                info("testFoo_LedManager", "Pass"); // log
                // register
                mgr.release(mListener);
            }
            
            @Override
            public String toString() {
                return mName;
            }
        },
        testGetEndoscopeId_EndoScopeManager("test get endoscope id@EndoScopeManager") {
            
            private EndoscopeManager.OnStatusListener mListener = new EndoscopeManager.OnStatusListener() {
                /**
                 * As following methods are invoked from EndoscopeManager controller
                 */
                @Override
                public void onError(String str) {
                    showToast(str);
                }
                
                @Override
                public void onInfo(String str) {
                    showToast(str);
                }
            };
        
            @Override
            public void process(Context ctx) {
                HpLog.i(TAG_ITEM, "[process] enter");
                if (ctx == null) {
                    throw new IllegalArgumentException("No context!!!");
                }
                
                EndoscopeManager mgr = EndoscopeManager.getInstance();
                mgr.init(mListener);
                int id = mgr.getEndoScopeId();
                Assert.assertEquals(0, id);
                // show result
                info("testGetEndoscopeId_EndoScopeManager", "Pass"); // log
                // register
                mgr.release(mListener);
            }
            
            @Override
            public String toString() {
                return mName;
            }
        },
        testLongPressPower_PhoneWindowManager("test enable shutdown property test") {
            
            private boolean mCanShutdown = true;
            
            @Override
            public void process(Context ctx) {
                HpLog.i(TAG_ITEM, "[process] enter");
                int value = (mCanShutdown == true)? 0: 1;
                
                // change the enable shutdown property
                SystemProperties.set("persist.vendor.med.system.shutdown", String.valueOf(value));
                int actualValue = SystemProperties.getInt("persist.vendor.med.system.shutdown", -1);
                Assert.assertEquals(value, actualValue);
                // show result
                String result = "actual value:" + String.valueOf(actualValue) +" Pass";
                showDailog(ctx, "testLongPressPower_PhoneWindowManager", result);
                
                // change
                this.mCanShutdown = (mCanShutdown == true)? false: true;
            }
            
            @Override
            public String toString() {
                return mName;
            }
        },
        Exit("Leave the test") {
            @Override
            public void process(Context ctx) {
                HpLog.i(TAG_ITEM, "[process] enter");
            }
            
            @Override
            public String toString() {
                return mName;
            }
        };
        
        private static final String TAG_ITEM = ItemContext.class.getSimpleName();
        
        protected String mName;
        
        private ItemContext(String name) {
            this.mName = name;
        }
        
        public String getName() {
            return this.mName;
        }
        
        public static ItemContext by(String name) {
            for (ItemContext item: ItemContext.values()) {
                if (item.mName == name) {
                    return item;
                }
            }
            
            return null;
        }
        
        private static void showDailog(Context ctx, String tag, String result) {
            // boundle context
            StringBuilder stb = new StringBuilder();
            stb.append("\n=====[").append(tag).append("]=====\n");
            stb.append("Result: ").append(result).append("\n");
            stb.append("===========================\n");
            String str = stb.toString();
            Log.i(TAG_ITEM, str);
            
            // show dailog
            AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(ctx);
            alertDialogBuilder.setMessage(str);
            alertDialogBuilder.setPositiveButton("yes", 
               new DialogInterface.OnClickListener() {
               @Override
               public void onClick(DialogInterface dialog, int id) {
                    // dismiss dialog
                    dialog.dismiss();
               }
              });

              AlertDialog alertDialog = alertDialogBuilder.create();
              alertDialog.show();
            
        }
        
        
        private static void info(String tag, String result) {
            StringBuilder stb = new StringBuilder();
            stb.append("\n=====[").append(tag).append("]=====\n");
            stb.append("Result: ").append(result).append("\n");
            stb.append("===========================\n");
            String str = stb.toString();
            Log.i(TAG_ITEM, str);
        }
        
        public abstract void process(Context ctx);
    }
    
    // item list
    List<String> mList = new ArrayList<>();
    
    // build item list
    {
        for (ItemContext item: ItemContext.values()) {
            this.mList.add(item.getName());
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        HpLog.i(TAG, "[onCreate] enter");

        CTX = getApplicationContext();

        String[] items = new String[this.mList.size()];
        items = this.mList.toArray(items);

        mAdapter = (ListAdapter) new ArrayAdapter<String>(this, 
                android.R.layout.simple_list_item_1,
                items);
        
        this.setListAdapter(mAdapter);
        
        
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        HpLog.i(TAG, "[onDestroy] enter");
    }

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        super.onListItemClick(l, v, position, id);
        HpLog.i(TAG, "onListItemClick: position[" + position + "]");
        String name = this.mList.get(position);
        ItemContext context = ItemContext.by(name);
        HpLog.i(TAG, "item context: " + context);
        if (context == null) {
            return;
        }
        
        if (context == ItemContext.Exit) {
            // finish
            this.finish();
        } else {
            context.process(this);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    /**
     * show information
     */
    private static void showToast(final String str) {
        HpLog.i(TAG, "[showToast] enter: " + str);
        // bundle data
        Bundle data = new Bundle();
        data.putString(KEY_INFO, str);
        
        Message msg = new Message();
        msg.what = SHOW_TOAST;
        msg.setData(data);
        sHandler.sendMessage(msg);
    }
}
