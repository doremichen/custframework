/**
 * Cust AIDL service header
 */
#ifndef _AIDL_SERVICE_H
#define _AIDL_SERVICE_H

#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <thread>
#include <chrono>
#include <atomic>
#include<condition_variable>

#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/IBinder.h>
#include <binder/Binder.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>

#include <com/cust/service/BnAIDLService.h>
#include <com/cust/service/BnAIDLServiceCallback.h>

using namespace android;
using namespace std;
using namespace com::cust::service;


#define SERVICE_NAME "native.cust.aidl.service"

namespace com {
namespace cust {
namespace service {
    class AIDLService : public BnAIDLService {
        public:
        static int instantiate();
        ::android::binder::Status setCallback(const ::android::sp<IAIDLServiceCallback>& cb);
        ::android::binder::Status init();
        ::android::binder::Status testCallback();
        static sp<IAIDLServiceCallback> sCallback;
    private:
        AIDLService();
        ~AIDLService();
    };
} // namespace service
} // namespace cust
} // namespace com
#endif /* _AIDL_SERVICE_H */