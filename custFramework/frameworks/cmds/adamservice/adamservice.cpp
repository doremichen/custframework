//********** COPYRIGHT 2013 altek Corporation *********************************
// FILE NAME:   atkservice.cpp
// VERSION:     $Revision: $
// DESCRIPTION: atkservice is command execute that can be used to test 
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
#define LOG_TAG "atkservice"
#include <cutils/log.h>

#define LOG_PRODTAG "Result"
 
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


#define LOGV(...) __android_log_print( ANDROID_LOG_VERBOSE, LOG_PRODTAG, __VA_ARGS__ )
#define LOGD(...) __android_log_print( ANDROID_LOG_DEBUG,  LOG_PRODTAG, __VA_ARGS__ )
#define LOGI(...) __android_log_print( ANDROID_LOG_INFO,  LOG_PRODTAG, __VA_ARGS__ )
#define LOGW(...) __android_log_print( ANDROID_LOG_WARN,  LOG_PRODTAG, __VA_ARGS__ )
#define LOGE(...) __android_log_print( ANDROID_LOG_ERROR,  LOG_PRODTAG, __VA_ARGS__ )


//#define DEBUG    //for log

using namespace android;

/*******************************************************************  
* mediaplayer
*/

sem_t wait_event;

sp<MediaPlayer> mediaplayer = NULL;
status_t mediastatus = UNKNOWN_ERROR;

void LoadSound(const char* file);
void PlayMusic();

//for music callbacks
class MPListener: public MediaPlayerListener
{
public:
    MPListener(); 
    ~MPListener();
    virtual void notify(int msg, int ext1, int ext2, const Parcel *obj = NULL);
//private:
           
};

MPListener::MPListener()
{
#ifdef DEBUG    
    printf("construct\n");
#endif    
}

void MPListener::notify(int msg, int ext1, int ext2, const Parcel *obj)
{
#ifdef DEBUG    
    printf("notify: msg = %d\n", msg);
#endif    
    switch(msg) {
    case MEDIA_PREPARED:
        
        break;    
    case MEDIA_PLAYBACK_COMPLETE:
        sem_post(&wait_event);  
        break;
    default:
        break;    
    }    
}        

MPListener::~MPListener()
{
#ifdef DEBUG    
    printf("destroy\n");
#endif    
}        
  
void LoadSound(const char* file)
{
#ifdef DEBUG    
   printf("[%s] ++\n", __func__);
#endif

#if TEST   
    const char *pSoundFileName;
    
   pSoundFileName= "/sdcard/music/testmp3.mp3";
#endif
   
   mediaplayer=new MediaPlayer();

   // create new listener and give it to MediaPlayer
    sp<MPListener> listener = new MPListener();
    mediaplayer->setListener(listener);
#if TEST		
   mediastatus = mediaplayer->setDataSource(pSoundFileName, NULL);
#else   
   mediastatus = mediaplayer->setDataSource(file, NULL);
#endif   

#ifdef DEBUG     
   printf("[%s]: setDataSource completed pSoundFileName: %s\n", __func__, file);
#endif
   
   if (mediastatus == NO_ERROR) {
//        mediaplayer->setAudioStreamType(AUDIO_STREAM_ENFORCED_AUDIBLE);
#ifdef DEBUG  
        printf("[%s]: mediaplayer start prepare\n", __func__);
#endif        
        mediastatus = mediaplayer->prepare();
        
   }
   else {
       printf("[%s]: mediaplayer prepare fail\n", __func__);
       return;
   } 
#ifdef DEBUG     
   printf("[%s]: prepare completed.\n", __func__);
   
   printf("[%s] --\n", __func__); 
#endif

}

void PlayMusic()
{
#ifdef DEBUG    
   printf("[%s] ++\n", __func__);
#endif
  
   if(mediaplayer == NULL) {
      printf("[%s]: mediaplayer == NULL\n", __func__);
      return;  
   } 
   
   
   if (mediastatus == NO_ERROR) {		    
        mediastatus = mediaplayer->start();
   }
   else {
       printf("[%s]: mediaplayer start fail\n", __func__);
       return;
   } 

#ifdef DEBUG   
   printf("[%s]: start completed.\n", __func__);
#endif 
   
   sem_wait(&wait_event);
   
   if (mediastatus == NO_ERROR) {		    
        mediaplayer->stop();
		mediaplayer->disconnect();       
   }

#ifdef DEBUG
   printf("[%s]--\n", __func__);
#endif
        
}    

//******* LOCAL CONSTANTS, TYPES, STRUCTURES, VARIABLES ***********************

/*******************************************************************  
* usbcdc
*/
#define USBCDC_DEV      "/dev/ttyGS0"

int main(int argc, char* const argv[])
{
    fflush(stdout);
    
    bool wantsUsage = false;
    int result = 0;
    
    while (1) {
        int ic = getopt(argc, argv, "h?");
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
        if (strcmp(argv[optind], "test") == 0) {
         
         aout << "test \n";
            
        }
        else if (strcmp(argv[optind], "mediaplayer") == 0) {
            optind++;
            if (strcmp(argv[optind], "play") == 0) {
                optind++;
                ProcessState::self()->startThreadPool();
                LoadSound(argv[optind]);
                PlayMusic();
            }                
        }     
        else if (strcmp(argv[optind], "usbcdc") == 0) {
            optind++;
            if (strcmp(argv[optind], "readsync") == 0) {
                char pcbuffer[255] = {0};
                int nusbcdcRead = 0;
                int usbcdc_fd = open(USBCDC_DEV,O_RDWR|O_NOCTTY);
                
                if(usbcdc_fd == -1) {	    
            	    printf("[%s], usbcdc_fd err=%d\n", __FUNCTION__, errno);
            	    wantsUsage = true;
                    result = 10;    
            	        
            	}
                
                while(1) {                    
                    printf("[%s]: please input number in pc consol.\n", __FUNCTION__);
                    memset(pcbuffer, 0, sizeof(pcbuffer));
                    nusbcdcRead = read(usbcdc_fd, pcbuffer, 255);
#ifdef DEBUG                    
                    printf("[%s]: pcbuffer = %s\n", __FUNCTION__, pcbuffer);
                    printf("[%s]: nusbcdcRead = %d, errno = %d\n", __FUNCTION__, nusbcdcRead, errno);
#endif                    
                    if(nusbcdcRead > 0) {
                       write(usbcdc_fd, pcbuffer, nusbcdcRead);    
                    }
                    
                }
                
            }
            else if (strcmp(argv[optind], "readasync") == 0) {
                
                char reply[32] = {0};
#ifdef TEST                
                char Outbuf[32] = {0};
#endif                
                int i = 0;

                int iOut, iIn, fd; // file descriptor
                
                fd = open(USBCDC_DEV, O_RDWR|O_NOCTTY|O_NONBLOCK);
                               
#ifdef TEST                
                for(i = 0; i < 30; ++i) {
                    Outbuf[i] = 'f';   
                }    
                
                Outbuf[30] = 0x0A;
                Outbuf[31] = 0x0D;
#endif                
                if(fd>0)
                {
                    printf("[%s]: Open successfully\n", __FUNCTION__);
                    while(1) {
#ifdef TEST                        
                        iOut = write(fd, Outbuf, sizeof(Outbuf));
                        iOut = write(fd, "hello hello IDN?\r\n", 17);
#endif                        
                        memset(reply, 0, 32);
                        iIn = read(fd, reply, 32);
                        
                        if(iIn > 0) {
                            iOut = write(fd, reply, iIn);
                        }    
#ifdef DEBUG                        
                        printf("[%s]: %s\n", __FUNCTION__, reply);
#endif                        
                    }
                    close(fd);
                }
                else
                {
                    printf("[%s]: failed to open device\n", __FUNCTION__);
                }
                
            }

        }
        else if (strcmp(argv[optind], "device") == 0) {
            optind++;
            if (strcmp(argv[optind], "autopressPK") == 0) {
                aout << "auto press power key !!!\n";
                optind++;
              if (optind < argc) {
                long times = strtol(argv[optind], NULL, 0);
                printf("max times: %ld\n", times);
                int id = 0;
                while (id < times) {
                    printf("press %d time\n", id+1);
                    system("input keyevent KEYCODE_POWER");
                    // execlp("input", "input", "keyevent", "KEYCODE_POWER", NULL);
                    sleep(3);
                    id++;
                    printf("next!!!\n");
                }
                

              }
              else {
                aerr << "atkservice: please input loop counter!!!" << endl;
                wantsUsage = true; 
                result = 10;           
              }
            }
            
        }
        else {
            aerr << "adamservice: Unknown command " << argv[optind] << "\n";
            wantsUsage = true;
            result = 10;
        }
    }
    
    if (wantsUsage) {
        aout << "Usage: adamservice [name] [opt]\n"
                "       name: h\n"
                "             mediaplayer\n"
                "             usbcdc\n"
                "             device\n"
                "       opt: h\n"
                "            play: mediaplayer play function.\n"
                "            readsync: test usbcdc read sync performance.\n"
                "            readasync: test usbcdc read async performance.\n"
                "            autopressPK: monitor auto press power key.\n"
                "Example:\n"
                "       adamservice device autopressPK 200: suto press power key 200 times.\n";
                
        return result;
    }
    
    return result;
}

