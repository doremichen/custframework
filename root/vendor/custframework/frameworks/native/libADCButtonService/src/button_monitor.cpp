/* ADC button monitor for Helios+
 *
 * This program is primarily intended as an example application.
 * Reads the current adc buttons status (ADC_SW_STATUS) from iio sysfs and starts adc alarm
 * to detect button state change.
 *
 */

#define LOG_NDEBUG 0  
#define LOG_TAG "button_monitor"
#include <cutils/log.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <poll.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/iio/events.h>
#include <linux/iio/types.h>

#include "ADCButtonService.h"

#ifndef DISABLE_LOG
/* macros */
#define pr_info(fmt,arg...)     ALOGI("%ld:info:[%s:%d] " fmt, clock(), __func__, __LINE__, ##arg)
#define pr_debug(fmt,arg...)    ALOGD("%ld:debug:[%s:%d] " fmt, clock(),__func__, __LINE__, ##arg)
#else
#define pr_info(fmt,arg...)
#define pr_debug(fmt,arg...)
#endif

// calculate elapsed time in ms
#define ELAPSED_TIME_US(now, start) (((now - start) * 1000 * 1000) / CLOCKS_PER_SEC)

#define noinline __attribute__((__noinline__))
#define __unused __attribute__((__unused__))

//#define TEST

/* constants */
typedef enum {
    SW_RELEASED = 0,
    SW1_PRESSED,
    SW2_PRESSED,
    ALL_SW_PRESSED,
    SW_UNKNOWN,
} ADC_SW_STATUS;

//map pool size
#define MAX_MAP_SIZE 65535u

using namespace android;

/*key value pair*/
typedef struct {
    ADC_SW_STATUS status;
    const char* name;
} Key_Value_Pair;

/* map */
typedef struct {
    Key_Value_Pair entries[MAX_MAP_SIZE];
    int size;
} Map;

//
// put data
//
static void 
map_put(Map* map, ADC_SW_STATUS key, const char* value) {
    if (map->size < MAX_MAP_SIZE) {
        map->entries[map->size].status = key;
        map->entries[map->size].name = strdup(value);
        map->size++;
    }
}

//
// get data by key
//
static const char* 
map_get(Map* map, ADC_SW_STATUS key) {
    for (int i = 0; i < map->size; i++) {
        if (map->entries[i].status == key) {
            return map->entries[i].name;
        }
    }
    return NULL;
}

//
// callback
//
BUTTON_EVENT g_eventFunc;

//
// thread body
//
static void*
monitor_proc(void *argc __unused)
{
    pr_info("[%s] enter\n", __FUNCTION__);

    Map map;
    map.size = 0;
        
    map_put(&map, SW_RELEASED, "SW_RELEASED");
    map_put(&map, SW1_PRESSED, "SW1_PRESSED");
    map_put(&map, SW2_PRESSED, "SW2_PRESSED");
    map_put(&map, ALL_SW_PRESSED, "ALL_SW_PRESSED");
    map_put(&map, SW_UNKNOWN, "SW_UNKNOWN");

    
    while (true) {
        
        const char* value = map_get(&map, ALL_SW_PRESSED);
        pr_info("current button status: %s\n", value);
        
        // callback to jni
        g_eventFunc(value);
        sleep(1); // delay 1 sec
    }

    pr_info("monitor_proc is finish!!!\n");
    pthread_exit(NULL);
    pr_info("[%s] exit\n", __FUNCTION__);
}

int start_detect_iio_event(BUTTON_EVENT eventFunc)
{
    pr_info("[%s] enter\n", __FUNCTION__);
   
    pr_info("hello buttun_monotor!!!");
    g_eventFunc = eventFunc;
    pr_info("[%s] start adc button monitor...\n", __FUNCTION__);
    pthread_t thread_id;
    int ret = pthread_create(&thread_id, NULL, monitor_proc, NULL);
    if (ret != 0) {
        pr_debug("Failed to create thread: error: %s\n", strerror(errno));
        return -1;
    }
    pr_info("[%s] exit\n", __FUNCTION__);
    //fprintf(stderr, "ADC Button Monitor is terminated: %d\n", ret);
    return 0;
}

