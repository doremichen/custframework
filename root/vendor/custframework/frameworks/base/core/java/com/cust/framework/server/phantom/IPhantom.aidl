/*
 * File: IPhatom.aidl
 * Brief: binder interface
 *
 * Author: 
 * Date:
 */
 
package com.cust.framework.server.phantom;

import  android.os.storage.DiskInfo;
import android.os.storage.VolumeInfo;


/**
 * {@hide}
 */
interface IPhantom
{
   void executeTouchEvent(in float x, in float y);
   
   void executeReboot();
   
   void executeShutdown();
   
   void doMasterClear();
   
   void addLocale(String localeId);
   
   void startStorageWizardFormatProgress(in DiskInfo diskinfo);
   
   void sendTouchEvent(boolean start);
   
   void getSizeByStatFs(in VolumeInfo info);
   
   void sendStorageEmptyInfo();
}
