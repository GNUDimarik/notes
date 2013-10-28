#include "Context.hpp"
#include "DroidBlaster.hpp"
#include "EventLoop.hpp"
#include "GraphicsService.hpp"
#include "TimeService.hpp"

void android_main(android_app* pApplication) {
    packt::TimeService lTimeService;
    packt::GraphicsService lGraphicsService(pApplication,
        &lTimeService);

    // Fills the context.
    packt::Context lContext = { &lGraphicsService, &lTimeService };

    packt::EventLoop lEventLoop(pApplication);
    dbs::DroidBlaster lDroidBlaster(&lContext);
    lEventLoop.run(&lDroidBlaster);
}
