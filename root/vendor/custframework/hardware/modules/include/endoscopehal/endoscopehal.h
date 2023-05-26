/**
 * File: endoscopehal.h
 * Breif: Declare the information about endoscopehal module
 *
 * Name: 
 * Date: 
 */

#ifndef ENDOSCOPE_HAL_H
#define ENDOSCOPE_HAL_H

#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <hardware/hardware.h>

__BEGIN_DECLS

/**
 * The id of this module
 */
#define ENDOSCOPE_MODULE_ID "endoscopehal"


/**
 * Every hardware module must have a data structure named HAL_MODULE_INFO_SYM
 * and the fields of this data structure must begin with hw_module_t
 * followed by module specific information.
 */
struct endoscope_module_t {
    struct hw_module_t common;
};


/**
 * Every device data structure must begin with hw_device_t
 * followed by module specific public methods and attributes.
 */
struct endoscope_device_t {
    struct hw_device_t common;

    //customize function
     int (*pGetSensorId)(struct endoscope_device_t *dev);
};


/** convenience API for opening and closing a device */

static inline int endoscope_hal_open(const struct hw_module_t* module, 
        struct endoscope_device_t** device) {
    return module->methods->open(module, "endos scope device",(struct hw_device_t**)device);
}

__END_DECLS

#endif  // DKI_INTERFACE_H
