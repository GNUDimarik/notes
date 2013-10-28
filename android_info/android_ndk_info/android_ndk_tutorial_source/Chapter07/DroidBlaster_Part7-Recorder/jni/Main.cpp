#include "Context.hpp"
#include "DroidBlaster.hpp"
#include "EventLoop.hpp"
#include "GraphicsService.hpp"
#include "SoundService.hpp"
#include "TimeService.hpp"

void android_main(android_app* pApplication) {
    packt::TimeService lTimeService;
    packt::GraphicsService lGraphicsService(pApplication,
        &lTimeService);
    packt::SoundService lSoundService(pApplication);

    // Fills the context.
    packt::Context lContext = { &lGraphicsService, &lSoundService,
        &lTimeService };

    // Starts the game loop.
    packt::EventLoop lEventLoop(pApplication);
    dbs::DroidBlaster lDroidBlaster(&lContext);
    lEventLoop.run(&lDroidBlaster);
}
