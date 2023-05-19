/**
 *  Copyright (C) xxxx
 */
package com.cust.server.serialport;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.RuntimeException;
import java.lang.IllegalArgumentException;

import com.med.hpframework.util.HpLog;

/**
 * SerialPort would probide the interface that access serail port.
 */
public final class SerialPort {
    private static final String TAG = SerialPort.class.getSimpleName();
    
    // load jni so file
    static {
        System.loadLibrary("serailport_jni");
    }
    
    /**
     * Singleton
     */
    private SerialPort() {}
     
    private static class Singleton {
        private static SerialPort sINSTANCE = new SerialPort();
    }
    
    public static SerialPort instance() {
        return Singleton.sINSTANCE;
    }
    
    private SerialPortCallback mCallback;
    
    // seriel port state
    enum STATE {
        OPEN,
        CLOSE
    }
    
    // init status
    private static STATE sStatus = STATE.CLOSE;
    
    // serial port config
    private interface CONFIG {
        File SERIAL_FILE = new File("/dev/tty");
        int BAUDRATE = 115200;
        int FLAG = 0;
    }
      
   
    // Callback
    public interface SerialPortCallback {
        
        void onResult(String info);
        
    }
    
    // -----------------------------------------------------------------
    // method
    public void registerCB(SerialPortCallback callback) {
        HpLog.i(TAG, "registerCB");
        HpLog.i(TAG, "cb: " + callback);
        this.mCallback = callback;
        HpLog.i(TAG, "registerCB successful...");
    }
    
    public void unregisterCB() {
        HpLog.i(TAG, "unregisterCB");
        this.mCallback = null;
        HpLog.i(TAG, "unregisterCB successful...");
    }
    
    public void triggerCB(String msg) {
        HpLog.i(TAG, "triggerCB");
        if (this.mCallback == null) {
            HpLog.i(TAG, "No callback!!!");
            return;
        }
        // invoke callback
        this.mCallback.onResult(msg);
        
    }
    
    public void openPort()
    {
        HpLog.i(TAG, "SerialPort initial!!!");
        
        if (sStatus == STATE.OPEN) {
            throw new RuntimeException("The port has been opened!!!");
        }
        
        _openSerialPort(CONFIG.SERIAL_FILE.getAbsolutePath(), CONFIG.BAUDRATE, CONFIG.FLAG);
        
        sStatus = STATE.OPEN;
    }
    
    public void closePort() {
        HpLog.i(TAG, "closeUart");
        if (sStatus == STATE.CLOSE) {
            throw new RuntimeException("The port has been closed!!!");
        }
        
        _closeSerialPort();
        sStatus = STATE.CLOSE;
    }
    
    public void writeCommand(byte[] datas) {
        HpLog.i(TAG, "writeCommand");
        // valid check
        if ((datas == null) || (datas.length == 0)) {
           throw new RuntimeException("The command datas is not avalible!!!");
        }
        
        if (sStatus == STATE.CLOSE) {
            throw new RuntimeException("The serial port is close!!!");
        }
        
        _sendCommand(datas);
    }

    /**
     * jni method section
     */
    private native void _openSerialPort(String path, int baudrate, int flags);
    private native void _closeSerialPort();
    private native void _sendCommand(byte[] datas);
    
    /**
     * invoke from jni
     */
     private void notifyFromNative(String msg) {
         HpLog.i(TAG, "[notifyFromNative]: msg = " + msg);
         
         // notify client
         //triggerCB(msg);
     }
     
}
