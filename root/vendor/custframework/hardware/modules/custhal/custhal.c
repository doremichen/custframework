/**
 * File: custhal.c
 * Breif: This is hal module.
 *
 * Name: Adam chen
 * Date: 
 */

#define LOG_NDEBUG 0
#define LOG_TAG "cust_device_hal"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <hardware/hardware.h>
#include <custhal/custhal.h>
#include <cutils/log.h>

// Device node
#define LIGHT_BRIGHTNESS "/sys/class/leds/lcd-backlight/brightness"

/**
 * hal device content
 */
typedef struct {
    struct cust_device_t device;
}cust_effects_context_t;

static int cust_device_open(const struct hw_module_t* module,
                         const char* name,
                         struct hw_device_t** device);
                         
                        
static int cust_device_close (struct hw_device_t* device)
{
    cust_effects_context_t *dev;
    ALOGV("[%s] enter\n", __FUNCTION__);
    dev = (cust_effects_context_t *)device;
    if(dev)
        free (dev);
    return 0;
}

static int sendcmd(int brightness)
{
     ALOGV("[%s] enter\n", __FUNCTION__);
     int fd, nwr;
     char lbuf[20];
     
     fd = open(LIGHT_BRIGHTNESS, O_RDWR);
     
     if(fd < 0) {
        ALOGE("[%s]: open device fail! err:[%s]", __FUNCTION__, strerror(errno));
        return errno;
     }
     
     nwr = sprintf(lbuf, "%d\n", brightness);
     write(fd, lbuf, nwr);
     
     close(fd);
     
 
     return 0; //success
}


static int cust_effects_device_on(struct cust_device_t *dev __unused)
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    int ret = sendcmd(220);
    return ret;
}

static int cust_effects_device_off(struct cust_device_t *dev __unused)
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    int ret = sendcmd(20);
    return ret;
}

/**
 * Initial cust hal device content
 */
static int cust_device_open(const struct hw_module_t* module,
              const char* name __unused,
              struct hw_device_t** device)
{
    ALOGV("[%s] enter\n", __FUNCTION__);
    cust_effects_context_t *dev;
    
    dev = (cust_effects_context_t *)malloc(sizeof(*dev));
    if (! dev) return EFAULT;  //Bad address
     
    dev->device.common.tag = HARDWARE_DEVICE_TAG;
    dev->device.common.version = 0; 
    dev->device.common.module = (struct hw_module_t *)module;
    dev->device.common.close = cust_device_close;
    
    dev->device.cust_effects_on = cust_effects_device_on;
    dev->device.cust_effects_off = cust_effects_device_off;
   
    *device = &dev->device.common;
    ALOGV("[%s] exit!\n", __FUNCTION__);
    
    return 0; // success
}


static struct hw_module_methods_t s_module_methods = {
    .open = cust_device_open
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

struct cust_module_t HAL_MODULE_INFO_SYM = { 
   
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = 1,
        .hal_api_version = 0,
        .id = CUST_HARDWARE_MODULE_ID,
        .name = "cust module name",
        .author = "AUTHOR_NAME",
        .methods = &s_module_methods,
    },

};
