/**
 * File: endoscopehal.c
 * Breif: This is hal module.
 *
 * Name: 
 * Date: 
 */

#define LOG_NDEBUG 0
#define LOG_TAG "endoscope_device_hal"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <hardware/hardware.h>
#include <endoscopehal/endoscopehal.h>
#include <cutils/log.h>

// Device node
#define ENDOSCOPE_PATH "/sys/class/leds/lcd-backlight/brightness"

/**
 * hal device content
 */
typedef struct {
    struct endoscope_device_t device;
}endoscope_context_t;

static int endoscope_device_open(const struct hw_module_t* module,
                         const char* name __unused,
                         struct hw_device_t** device);
                         
                        
static int endoscope_device_close (struct hw_device_t* device)
{
    endoscope_context_t *dev;
    ALOGV("[%s] enter\n", __FUNCTION__);
    dev = (endoscope_context_t *)device;
    if(dev)
        free (dev);
    return 0;
}

static int getSensorId(struct endoscope_device_t *dev __unused)
{
    ALOGV("[%s] enter\n", __FUNCTION__);

    return 0;
}

/**
 * Initial cust hal device content
 */
static int endoscope_device_open(const struct hw_module_t* module,
              const char* name __unused,
              struct hw_device_t** device)
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    endoscope_context_t *dev;
    
    dev = (endoscope_context_t *)malloc(sizeof(*dev));
    if (! dev) return EFAULT;  //Bad address
     
    dev->device.common.tag = HARDWARE_DEVICE_TAG;
    dev->device.common.version = 0; 
    dev->device.common.module = (struct hw_module_t *)module;
    dev->device.common.close = endoscope_device_close;
    
    dev->device.pGetSensorId = getSensorId;

   
    *device = &dev->device.common;
    ALOGV("[%s] exit!\n", __FUNCTION__);
    
    return 0; // success
}


static struct hw_module_methods_t s_module_methods = {
    .open = endoscope_device_open
};

/**
*
const struct cust_module_t HAL_MODULE_INFO_SYM = {   
 common: {
        tag: HARDWARE_MODULE_TAG,
        version_major: 1,
        version_minor: 0,
        id: EFFECTS_HARDWARE_MODULE_ID,
        name: "FIH eft module",
        author: "BoWeiChen",
        methods: &s_module_methods,
        reserved: {}
    },
};    
   this body is old style, not fit the new android version. 
*  this module new body is as following.
*/

struct endoscope_module_t HAL_MODULE_INFO_SYM = { 
   
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = 1,
        .hal_api_version = 0,
        .id = ENDOSCOPE_MODULE_ID,
        .name = "endoscope device",
        .author = "medical",
        .methods = &s_module_methods,
    },

};
