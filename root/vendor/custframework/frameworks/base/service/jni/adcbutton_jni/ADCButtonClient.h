/**
 * File: ADCButtonClient.h
 * Breif: Declare Client class
 *
 * Name: 
 * Date: 
 */

#ifndef NATIVE_ADC_BUTTON_CLIENT_H  
#define NATIVE_ADC_BUTTON_CLIENT_H  

#include <utils/RefBase.h>

#include "ADCButtonCallback.h"

using namespace std;
using namespace com::med::adc::button::service;
  
namespace android  
{  
class ADCButtonClient : public RefBase
{  
public:

     virtual ~ADCButtonClient();
     ADCButtonClient();
     static sp<IADCButtonService> BpADCButtonService;
     static  const sp<IADCButtonService> getADCButtonService();
     
     int init_module();
     void setADCButtonCallback(const sp<IADCButtonCallback>& callback);
     
     void exit();
                 
};  

}; //namespace   
#endif // NATIVE_ADC_BUTTON_CLIENT_H  