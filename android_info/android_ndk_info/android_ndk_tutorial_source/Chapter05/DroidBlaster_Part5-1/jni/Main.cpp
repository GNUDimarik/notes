#include "EventLoop.hpp"

void android_main(android_app* pApplication) {
    packt::EventLoop lEventLoop(pApplication);
    lEventLoop.run();
}
