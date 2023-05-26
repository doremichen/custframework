// File: ads7914_service.cpp
#define LOG_NDEBUG 0  
#define LOG_TAG "adcmonitord"
#include <cutils/log.h>

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <utils/Log.h>

#include <sys/ioctl.h>
#include <linux/iio/events.h>
#include <poll.h>
#include <pthread.h>

typedef enum {
    False = 0,
    True = 1,
} Bool;


typedef struct _IIODevice {
    long expected;
    const char *filePath;
} IIODevice;

IIODevice device2[] ={
    {-1L, "/sys/bus/iio/devices/iio:device0/in_temp_bat_therm_30k_input"},
    {-1L, "/sys/bus/iio/devices/iio:device2/in_voltage0_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device2/in_voltage1_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device2/in_voltage2_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device2/in_voltage3_raw"},
};

IIODevice device3[] ={
    {-1L, "/sys/bus/iio/devices/iio:device3/in_voltage0_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device3/in_voltage1_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device3/in_voltage2_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device3/in_voltage3_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device3/in_voltage4_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device3/in_voltage5_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device3/in_voltage6_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device3/in_voltage7_raw"},
};

IIODevice device4[] ={
    {-1L, "/sys/bus/iio/devices/iio:device4/in_voltage0_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device4/in_voltage1_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device4/in_voltage2_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device4/in_voltage3_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device4/in_voltage4_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device4/in_voltage5_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device4/in_voltage6_raw"},
    {-1L, "/sys/bus/iio/devices/iio:device4/in_voltage7_raw"},
};

#define sizeOf(arr) sizeof(arr)/sizeof(IIODevice)

static const char* const IIO_EVENT_PATH = "/dev/iio:device0";

int open_file(const char *fileName)
{
    //ALOGI("[%s]\n", __FUNCTION__);
    int fd;
    
    fd = open(fileName, O_RDWR);
    if (fd == -1) {
        ALOGE("open fail!!!");
    }
    return fd;
}

void close_file(int fd)
{
    //ALOGI("[%s]\n", __FUNCTION__);
    if (close(fd) != 0)
    {
        ALOGE("close fail!!!");
    }
}

void write_file(int fd, unsigned char val)
{
    //ALOGI("[%s]\n", __FUNCTION__);
    ssize_t ret;
    
    ret = write(fd, &val, 1);
    if (ret <= 0) {
        ALOGE("write fail!!!");
    }
}

long read_file(int fd)
{
    //ALOGI("[%s]\n", __FUNCTION__);
    ssize_t ret;
    const char buf[16] = {'\0'}, *p;
    
    ret = read(fd, const_cast<char *>(buf), sizeof(buf));
    
    if (ret > 0) {
        p = buf;
        while (ret--) {
            // show
            ALOGI("%d ", *p++);
        }
        ALOGI("\n");
        
        // to integer
        auto value = strtol(buf, nullptr, 16);
        ALOGI("value: %#04lx\n", value);
        return value;
    } 
    else {
        ALOGE("read fail!!!");
    }
    return 0L;
}

static Bool check(const IIODevice *pIIODevice, size_t size) {
    ALOGI("[%s]\n", __FUNCTION__);
    Bool ret = True;
    int fd;
    unsigned i;
    
    ALOGI("=== start ========================================\n");
    for (i = 0; i < size; i++) {
        ALOGI("[path]: %s ===\n", pIIODevice[i].filePath);
        // open
        fd = open_file(pIIODevice[i].filePath);
        // read
        auto value = read_file(fd);
        
        // check
        if (value != pIIODevice[i].expected) {
            ALOGE("CHECK FAILE!!!");
            ret = False;
            // close
            //close_file(fd);
            //break;
        }
        // close
        close_file(fd);
        ALOGI("==============\n");
    }
    ALOGI("=== end =========================================\n");
    return ret;
}

//
// Read iio data and check
//
static void getIIOState()
{
    // check
    check(device2, sizeOf(device2));
    check(device3, sizeOf(device3));
    check(device4, sizeOf(device4));
}

//
// iio event detect task
//
void* 
iio_event_detect_task(void* arg)
{
    ALOGI("[%s] enter\n", __FUNCTION__);
    int iio_fd = open(IIO_EVENT_PATH, O_RDONLY);
    if (iio_fd < 0) {
        ALOGE("Failed to open IIO device: %s\n", strerror(errno));
        return NULL;
    }

    int event_fd;
    int ret = ioctl(iio_fd, IIO_GET_EVENT_FD_IOCTL, &event_fd);
    if (ret < 0) {
        ALOGE("ioctl failed: %s\n", strerror(errno));
        close(iio_fd);
        return NULL;
    }

    ALOGI("IIO event file descriptor: %d\n", event_fd);

    struct pollfd fds;
    fds.fd = event_fd;
    fds.events = POLLIN;

    while (1) {
        ret = poll(&fds, 1, -1);
        if (ret < 0) {
            ALOGE("poll failed: %s\n", strerror(errno));
            break;
        }
        if (ret > 0) {
            if (fds.revents & POLLIN) {
                // read iio event
                struct iio_event_data event;
                ssize_t bytes_read = read(event_fd, &event, sizeof(event));
                if (bytes_read == sizeof(event)) {
                    // TODO: process iio event
                    ALOGI("Received IIO event: %llu\n", event.id);
                }
            }
        }
    }

    close(event_fd);
    close(iio_fd);
    ALOGI("[%s] task finish\n", __FUNCTION__);
    pthread_exit(NULL);
}

//
// entry
//
int main(int argc, char* argv[])
{  
    ALOGI("ADCmonitor deamon start...\n");
    int interval = 10;
    
    // start thread to detect iio event
    pthread_t thread_id;
    
    int ret = pthread_create(&thread_id, NULL, iio_event_detect_task, NULL);
    if (ret != 0) {
        ALOGE("Failed to create thread: %s\n", strerror(errno));
        return -1;
    }
    
    // live
    while (true) 
    {
        // get iio state
        getIIOState();
        sleep(interval);
    };
    
    return 0;
}  