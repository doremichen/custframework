/**
 * @Copyright xxxx
 */

package com.cust.framework.server.dicom;

import com.cust.framework.util.Print;



public enum DICOMContext {

    INSTANCE;


    private static final String TAG = DICOMContext.class.getSimpleName();


    public synchronized void send(byte[] request) {
        Print.i(TAG,"send");

    }
    
    public synchronized byte[] receive() {
        Print.i(TAG,"receive");
        return null;
    }
}