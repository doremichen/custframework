/**
 * @Copyright xxxx
 */

package com.med.hpframework.server.dicom;

import com.med.hp.server.HpSystemCoreServer;
import com.med.hpframework.util.HpLog;

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
        HpLog.i(TAG,"onStart");
        
        // TODO: test
        Jpg2Dcm jpg2Dcm = new Jpg2Dcm();  //
//        try {
//            jpg2Dcm.convert("/AA.jpg", "/AA.dcm");   // Only Convert Jpg to Dicom without seting any dicom tag (without DicomHeader)
//        } catch (IOException e) {
//            e.printStackTrace();
//            HpLog.e(TAG, e.getMessage());
//        }
    }

}