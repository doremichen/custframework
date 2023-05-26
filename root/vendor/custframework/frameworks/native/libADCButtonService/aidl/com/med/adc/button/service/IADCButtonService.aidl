//
// ADC button service
//
package com.med.adc.button.service;
  
import com.med.adc.button.service.IADCButtonCallback;  

interface IADCButtonService {
    void init();
    void setCallback(IADCButtonCallback cb);
}
