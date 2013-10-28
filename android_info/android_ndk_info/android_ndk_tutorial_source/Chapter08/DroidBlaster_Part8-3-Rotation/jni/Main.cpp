#include "Configuration.hpp"
#include "Context.hpp"
#include "DroidBlaster.hpp"
#include "EventLoop.hpp"
#include "GraphicsService.hpp"
#include "InputService.hpp"
#include "Sensor.hpp"
#include "SoundService.hpp"
#include "TimeService.hpp"
#include "Log.hpp"

void android_main(android_app* pApplication) {
    packt::EventLoop lEventLoop(pApplication);
    packt::Configuration lConfiguration(pApplication);
    // Creates input devices.
    packt::Sensor lAccelerometer(lEventLoop,
        ASENSOR_TYPE_ACCELEROMETER);

    // Creates services.
    packt::TimeService lTimeService;
    packt::GraphicsService lGraphicsService(pApplication,
        &lTimeService);
    packt::InputService lInputService(pApplication, &lConfiguration,
        &lAccelerometer, lGraphicsService.getWidth(),
        lGraphicsService.getHeight());
    packt::SoundService lSoundService(pApplication);

    // Fills the context.
    packt::Context lContext = { &lGraphicsService, &lInputService,
        &lSoundService, &lTimeService };

    // Starts the game loop.
    dbs::DroidBlaster lDroidBlaster(&lContext);
    lEventLoop.run(&lDroidBlaster, &lInputService);
}
