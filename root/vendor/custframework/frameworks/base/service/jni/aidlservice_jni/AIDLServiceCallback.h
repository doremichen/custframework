/**
 * AIDL Service Callback
 */
#ifndef _AIDL_SERVICE_CALLBACK_H
#define _AIDL_SERVICE_CALLBACK_H

#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

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

namespace com {
namespace cust {
namespace service {
    class AIDLServiceCallback : public BnAIDLServiceCallback {
    public:
        static sp<AIDLServiceCallback> getInstance();
        virtual ::android::binder::Status onCallback(const ::android::String16& event);
    private:
           AIDLServiceCallback();
           ~AIDLServiceCallback();
    };
} // namespace service
} // namespace cust
} // namespace com
#endif /* _AIDL_SERVICE_CALLBACK_H */
