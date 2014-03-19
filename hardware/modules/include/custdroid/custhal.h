#ifndef CUST_HAL_H
#define CUST_HAL_H

#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <hardware/hardware.h>

__BEGIN_DECLS

/**
 * The id of this module
 */
#define CUST_HARDWARE_MODULE_ID "custhal"


/**
 * Every hardware module must have a data structure named HAL_MODULE_INFO_SYM
 * and the fields of this data structure must begin with hw_module_t
 * followed by module specific information.
 */
struct cust_module_t {
    struct hw_module_t common;
};


/**
 * Every device data structure must begin with hw_device_t
 * followed by module specific public methods and attributes.
 */
struct cust_device_t {
    struct hw_device_t common;

    //customize function
     int (*cust_effects_on)(struct cust_device_t *dev);
     int (*cust_effects_off)(struct cust_device_t *dev);
};


/** convenience API for opening and closing a device */

static inline int cust_effects_open(const struct hw_module_t* module, 
        struct cust_device_t** device) {
    return module->methods->open(module, "",(struct hw_device_t**)device);
}

__END_DECLS

#endif  // DKI_INTERFACE_H
