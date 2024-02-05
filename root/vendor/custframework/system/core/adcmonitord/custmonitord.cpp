// File: ads7914_service.cpp
#define LOG_NDEBUG 0  
#define LOG_TAG "custmonitord"
#include <cutils/log.h>

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <utils/Log.h>

#include <pthread.h>


//
// work task
//
void* 
work_task(void* arg)
{
    ALOGI("[%s] enter\n", __FUNCTION__);
    int interval = 3;
    while (1) {
        ALOGI("[%s] task run!!!\n", __FUNCTION__);
        sleep(interval);
    }
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
    
    int ret = pthread_create(&thread_id, NULL, work_task, NULL);
    if (ret != 0) {
        ALOGE("Failed to create thread: %s\n", strerror(errno));
        return -1;
    }
    
    // live
    while (true) 
    {
        sleep(interval);
    };
    
    return 0;
}  