#include "EventLoop.hpp"
#include "Log.hpp"

namespace packt {
    EventLoop::EventLoop(android_app* pApplication) :
        mApplication(pApplication)
    {}

    void EventLoop::run() {
        int32_t lResult;
        int32_t lEvents;
        android_poll_source* lSource;

        // Makes sure native glue is not stripped by the linker.
        app_dummy();

        // Global step loop.
        packt::Log::info("Starting event loop");
        while (true) {
            // Event processing loop.
            while ((lResult = ALooper_pollOnce(-1, NULL, &lEvents,
                                         (void**) &lSource)) >= 0) {
                // An event has to be processed.
                if (lSource != NULL) {
                    packt::Log::info("Processing an event");
                    lSource->process(mApplication, lSource);
                }
                // Application is getting destroyed.
                if (mApplication->destroyRequested) {
                    packt::Log::info("Exiting event loop");
                    return;
                }
            }
        }
    }
}
