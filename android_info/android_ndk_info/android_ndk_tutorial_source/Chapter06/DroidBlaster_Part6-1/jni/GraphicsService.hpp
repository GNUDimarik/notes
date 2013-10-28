#ifndef _PACKT_GRAPHICSSERVICE_HPP_
#define _PACKT_GRAPHICSSERVICE_HPP_

#include "TimeService.hpp"
#include "Types.hpp"

#include <android_native_app_glue.h>
#include <EGL/egl.h>

namespace packt {
    class GraphicsService {
    public:
        GraphicsService(android_app* pApplication,
                        TimeService* pTimeService);

        const int32_t& getHeight();
        const int32_t& getWidth();

        status start();
        void stop();
        status update();

    private:
        android_app* mApplication;
        TimeService* mTimeService;

        // Display properties.
        int32_t mWidth, mHeight;
        EGLDisplay mDisplay;
        EGLSurface mSurface;
        EGLContext mContext;
    };
}
#endif
