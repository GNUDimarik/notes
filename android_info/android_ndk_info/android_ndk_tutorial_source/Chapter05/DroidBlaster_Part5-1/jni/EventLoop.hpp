#ifndef _PACKT_EVENTLOOP_HPP_
#define _PACKT_EVENTLOOP_HPP_

#include "Types.hpp"

#include <android_native_app_glue.h>

namespace packt {
    class EventLoop {
    public:
        EventLoop(android_app* pApplication);

        void run();

    private:
        // Application details provided by Android.
        android_app* mApplication;
    };
}
#endif
