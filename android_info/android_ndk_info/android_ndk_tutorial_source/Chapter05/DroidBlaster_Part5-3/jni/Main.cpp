#include "Context.hpp"
#include "DroidBlaster.hpp"
#include "EventLoop.hpp"
#include "TimeService.hpp"

void android_main(android_app* pApplication) {
    packt::TimeService lTimeService;
    packt::Context lContext = { &lTimeService };

    packt::EventLoop lEventLoop(pApplication);
    dbs::DroidBlaster lDroidBlaster(lContext, pApplication);
    lEventLoop.run(&lDroidBlaster);
}
