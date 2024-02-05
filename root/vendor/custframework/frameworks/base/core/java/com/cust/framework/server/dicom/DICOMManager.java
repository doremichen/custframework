/**
 * @Copyright xxxx
 */

package com.cust.framework.server.dicom;

import com.cust.system.server.HpSystemCoreServer;
import com.cust.framework.util.Print;

import java.io.IOException;

import org.dcm4che3.data.Tag;
import org.dcm4che3.data.VR;
import org.dcm4che3.io.DicomInputStream;
import org.dcm4che3.io.DicomOutputStream;
import org.dcm4che3.tool.jpg2dcm.Jpg2Dcm;


/**
 * The DICOMManager would transit the data to PACS server
 */
public class DICOMManager extends HpSystemCoreServer {
    private static final String TAG = DICOMManager.class.getSimpleName();

    @Override
    public void onStart() {
        Print.i(TAG,"onStart");
        
        // TODO: test
        Jpg2Dcm jpg2Dcm = new Jpg2Dcm();  //
//        try {
//            jpg2Dcm.convert("/AA.jpg", "/AA.dcm");   // Only Convert Jpg to Dicom without seting any dicom tag (without DicomHeader)
//        } catch (IOException e) {
//            e.printStackTrace();
//            Print.e(TAG, e.getMessage());
//        }
    }

}