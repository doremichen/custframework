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

#include <utils/TextOutput.h>

#include <getopt.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>

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
    int retval;
 
    if ((retval = __reboot(LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2,
            mode, NULL)) < 0) {
        LOGE("Try to shutdown the machine failed!");
        exit(EXIT_FAILURE);
    }

}

static void hello()
{
    aout << "Hello world!!! \n";
}

static void shutDownDevice()
{
    rebootDevice(RB_POWER_OFF);
}

static void rebootDevice()
{
    rebootDevice(RB_AUTOBOOT);
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
            aerr << "atkservice: Unknown option -" << ic << "\n";
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
            aerr << "atkservice: Unknown command " << argv[optind] << "\n";
            wantsUsage = true;
            result = 10;
        }

    }
    
    if (wantsUsage) {
        aout << "Usage: toolkits [name] [opt]\n"
                "       name: h help infomation\n"
                "             test show hello world\n"
                "             autopressP auto press power button\n"
                "             pressP press power button\n"
                "             runM run Monkey\n"
                "             shutdown shutdown device\n"
                "             reboot reboot device\n"
                "       opt: h\n"
                "Example:\n"
                ;
                
        return result;
    }
    
    return result;
}

