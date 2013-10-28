#include "Context.hpp"
#include "DroidBlaster.hpp"
#include "EventLoop.hpp"
#include "GraphicsService.hpp"
#include "InputService.hpp"
#include "JNI.hpp"
#include "Keyboard.hpp"
#include "SoundService.hpp"
#include "TimeService.hpp"

void android_main(android_app* pApplication) {
    // Attaches to the VM. Only necessary for the Keyboard class.
    // Automatically detaches from the VM when android_main() exits.
    packt::JNI lJNI(pApplication);
    // Creates input devices.
    packt::Keyboard lKeyboard(&lJNI);

    // Creates services.
    packt::TimeService lTimeService;
    packt::GraphicsService lGraphicsService(pApplication,
        &lTimeService);
    packt::InputService lInputService(pApplication, &lKeyboard,
        lGraphicsService.getWidth(), lGraphicsService.getHeight());
    packt::SoundService lSoundService(pApplication);

    // Fills the context.
    packt::Context lContext = { &lGraphicsService, &lInputService,
        &lSoundService, &lTimeService };

    // Starts the game loop.
    packt::EventLoop lEventLoop(pApplication);
    dbs::DroidBlaster lDroidBlaster(&lContext);
    lEventLoop.run(&lDroidBlaster, &lInputService);
}
