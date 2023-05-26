/**
 * @Copyright xxxx
 */

package com.med.hpframework.server.dicom;

import com.med.hpframework.util.HpLog;



public enum DICOMContext {

    INSTANCE;


    private static final String TAG = DICOMContext.class.getSimpleName();


    public synchronized void send(byte[] request) {
        HpLog.i(TAG,"send");

    }
    
    public synchronized byte[] receive() {
        HpLog.i(TAG,"receive");
        return null;
    }
}