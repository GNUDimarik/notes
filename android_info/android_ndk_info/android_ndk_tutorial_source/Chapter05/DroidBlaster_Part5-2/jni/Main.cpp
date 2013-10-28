#include "DroidBlaster.hpp"
#include "EventLoop.hpp"

void android_main(android_app* pApplication) {
    packt::EventLoop lEventLoop(pApplication);
    dbs::DroidBlaster lDroidBlaster;
    lEventLoop.run(&lDroidBlaster);
}
