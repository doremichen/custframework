//********** COPYRIGHT 2013 altek Corporation *********************************
// FILE NAME:   toolkits.cpp
// VERSION:     $Revision: $
// DESCRIPTION: toolkits is command execute that can be used to test 
//              hardware function.
//
//
//*****************************************************************************
// UPDATE LOG:
// $Log: $
//*****************************************************************************

//****************** STANDARD Cpp-INCLUDE FILES *********************************
//************* GLOBAL AND PROJECT INCLUDE FILES ******************************
#define LOG_NDEBUG 0  
#define LOG_TAG "toolkits"
#include <cutils/log.h>
#include <sys/reboot.h>

#include <getopt.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>
#include <errno.h>

#include <binder/ProcessState.h>
#include <media/mediaplayer.h>
#include <media/MediaPlayerInterface.h>
#include <system/audio.h>

#include <android/log.h>

 
#define LOGV(...) __android_log_print( ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__ )
#define LOGD(...) __android_log_print( ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )
#define LOGI(...) __android_log_print( ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__ )
#define LOGW(...) __android_log_print( ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__ )
#define LOGE(...) __android_log_print( ANDROID_LOG_ERROR,  LOG_TAG, __VA_ARGS__ )


extern "C" int __reboot(int, int, int, void*);

//#define DEBUG    //for log

using namespace android;

void pressPowerButton() {
    printf("pressPowerButton!!!\n");
    system("input keyevent 26");
}


void AutoPressPowerButton() {
    
    printf("AutoPressPowerButton!!!\n");
    
    pid_t pid;
    
    pid = fork();
    
    if(pid == 0) {
        while(true) {
            system("input keyevent 26");
            sleep(3);
        }
    }
    
/*     pid_t pid;
    
    pid = fork();
    
    if(pid == 0) {
        // child process
        printf("child process, pid = %u\n",getpid());
        printf("parent of child process, pid = %u\n",getppid());
        printf("Modidy 8\n");
        
        char * argv_list[] = {"input","keyevent","26",NULL};
        
        // execute
        execvp("input",argv_list);
        exit(0);
    } */
}

void executeMonkey() {
    printf("execute monkey\n");
    pid_t pid;
    
    pid = fork();
    
    if(pid == 0) {
        system("/system/bin/run.sh");
    }
}


void rebootDevice(int mode)
{
#if ANDROID4_2
    int retval;
 
    if ((retval = __reboot(LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2,
            mode, NULL)) < 0) {
        LOGE("Try to shutdown the machine failed!");
        exit(EXIT_FAILURE);
    }
#else
    reboot(mode);
#endif
}


static void hello()
{
    printf("Hello world!!! \n");
}

static void shutDownDevice()
{
    rebootDevice(RB_POWER_OFF);
}

static void rebootDevice()
{
    rebootDevice(RB_AUTOBOOT);
}

/**
 * Driver node info
 */
const char *DriveInfo[] = {
    "/sys/bus/iio/devices/iio:device2/in_voltage0_raw",
    "/sys/bus/iio/devices/iio:device2/in_voltage1_raw",
    "/sys/bus/iio/devices/iio:device2/in_voltage2_raw",
    "/sys/bus/iio/devices/iio:device2/in_voltage3_raw",
};

#define sizeOf(arr) sizeof(arr)/sizeof(&arr[0])

int open_file(const char *fileName)
{
    int fd;
    
    fd = open(fileName, O_RDWR);
    if (fd == -1) {
        perror("open fail!!!");
    }
    return fd;
}

void close_file(int fd)
{
    if (close(fd) != 0)
    {
        perror("close fail!!!");
    }
}

void write_file(int fd, unsigned char val)
{
    ssize_t ret;
    
    ret = write(fd, &val, 1);
    if (ret <= 0) {
        perror("write fail!!!");
    }
}

void read_file(int fd)
{
    ssize_t ret;
    const char buf[16] = {'\0'}, *p;
    
    ret = read(fd, const_cast<char *>(buf), sizeof(buf));
    
    if (ret > 0) {
        p = buf;
        while (ret--) {
            // show
            printf("%d ", *p++);
        }
        printf("\n");
        
        // to integer
        auto value = std::strtol(buf, nullptr, 16);
        printf("value: %#04lx\n", value);
        
    } 
    else {
        perror("read fail!!!");
    }

}


static void getIIOState()
{
    int fd;
    unsigned i;
    size_t number = sizeOf(DriveInfo);
    
    // look up driver
    for (i = 0; i < number; i++) {
        printf("[path]: %s ===\n", DriveInfo[i]);
        // open
        fd = open_file(DriveInfo[i]);
        // read
        read_file(fd);
        // close
        close_file(fd);
        printf("==============\n");
    }
}


struct action {
    const char *arg;
    void (*pfunc)();
} actMap[] = {
    {"test", hello},
    {"shutdown", shutDownDevice},
    {"reboot", rebootDevice},
    {"autopressP", AutoPressPowerButton},
    {"pressP", pressPowerButton},
    {"runM", executeMonkey},
    {"getIIO", getIIOState},
    {NULL, NULL},
};


int main(int argc, char* const argv[])
{
    fflush(stdout);
    
    bool wantsUsage = false;
    int result = 0;
    
    while (1) {
        int ic = getopt(argc, argv, "h?");
        printf("ic: %d\n", ic);
        if (ic < 0)
            break;

        switch (ic) {
        case 'h':
        case '?':
            wantsUsage = true;
            break;
        default:
            printf("atkservice: Unknown option - %d\n", ic);
            wantsUsage = true;
            result = 10;
            break;
        }
    }
    
    if (optind >= argc) {
        wantsUsage = true;
    } 
    else if (!wantsUsage) {
        bool validArg = false;
        // look up table
        for (unsigned int i = 0; actMap[i].arg != NULL; i++) {
            if (strcmp(argv[optind], actMap[i].arg) == 0) {
                validArg = true;
                // call function
                actMap[i].pfunc();
                break;
            }
        }
        
        if (!validArg) {
            printf("atkservice: Unknown command: %s\n", argv[optind]);
            wantsUsage = true;
            result = 10;
        }

    }
    
    if (wantsUsage) {
        printf("Usage: toolkits [name] [opt]\n");
        printf("       name: h help infomation\n");
        printf("             test show hello world\n");
        printf("             autopressP auto press power button\n");
        printf("             pressP press power button\n");
        printf("             runM run Monkey\n");
        printf("             shutdown shutdown device\n");
        printf("             reboot reboot device\n");
        printf("             getIIO get io driver\n");
        printf("       opt: h\n");
        printf("Example:toolkits test\n");
                
        return result;
    }
    
    return result;
}

