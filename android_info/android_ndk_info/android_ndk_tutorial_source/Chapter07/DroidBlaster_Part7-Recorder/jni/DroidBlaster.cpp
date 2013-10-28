#include "DroidBlaster.hpp"
#include "Log.hpp"

namespace dbs {
    DroidBlaster::DroidBlaster(packt::Context* pContext) :
        mGraphicsService(pContext->mGraphicsService),
        mSoundService(pContext->mSoundService),
        mTimeService(pContext->mTimeService),
        mBackground(pContext), mShip(pContext),
        mStartSound(mSoundService->registerSound("start.pcm")) {
        packt::Log::info("Creating DroidBlaster");
    }

    packt::status DroidBlaster::onActivate() {
        packt::Log::info("Activating DroidBlaster");

        // Starts services.
        if (mGraphicsService->start() != packt::STATUS_OK) {
            return packt::STATUS_KO;
        }
        if (mSoundService->start() != packt::STATUS_OK) {
            return packt::STATUS_KO;
        }

        // Starts background music.
        mSoundService->playBGM("bgm.mp3");
        mSoundService->recordSound();
        mSoundService->playSound(mStartSound);
        // Initializes game objects.
        mBackground.spawn();
        mShip.spawn();

        mTimeService->reset();
        return packt::STATUS_OK;
    }

    void DroidBlaster::onDeactivate() {
        packt::Log::info("Deactivating DroidBlaster");
        mGraphicsService->stop();
        mSoundService->stop();
    }

    packt::status DroidBlaster::onStep() {
        mTimeService->update();

        // Updates entities.
        mBackground.update();

        // Updates services.
        if (mGraphicsService->update() != packt::STATUS_OK) {
            return packt::STATUS_KO;
        }
        return packt::STATUS_OK;
    }

    void DroidBlaster::onStart() {
        packt::Log::info("onStart");
    }

    void DroidBlaster::onResume() {
        packt::Log::info("onResume");
    }

    void DroidBlaster::onPause() {
        packt::Log::info("onPause");
    }

    void DroidBlaster::onStop() {
        packt::Log::info("onStop");
    }

    void DroidBlaster::onDestroy() {
        packt::Log::info("onDestroy");
    }

    void DroidBlaster::onSaveState(void** pData, size_t* pSize) {
        packt::Log::info("onSaveInstanceState");
    }

    void DroidBlaster::onConfigurationChanged() {
        packt::Log::info("onConfigurationChanged");
    }

    void DroidBlaster::onLowMemory() {
        packt::Log::info("onLowMemory");
    }

    void DroidBlaster::onCreateWindow() {
        packt::Log::info("onCreateWindow");
    }

    void DroidBlaster::onDestroyWindow() {
        packt::Log::info("onDestroyWindow");
    }

    void DroidBlaster::onGainFocus() {
        packt::Log::info("onGainFocus");
    }

    void DroidBlaster::onLostFocus() {
        packt::Log::info("onLostFocus");
    }
}
