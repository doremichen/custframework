/*
 * File: IServiceCallBack.aidl
 * Brief: This is aidl interface
 *
 * Author: Adam chen
 * Date: 109/11/28
 */
 
package custdroid.hardware;


/**
 * {@hide}
 */
oneway interface IServiceCallBack
{
    void info(String msg);
}
