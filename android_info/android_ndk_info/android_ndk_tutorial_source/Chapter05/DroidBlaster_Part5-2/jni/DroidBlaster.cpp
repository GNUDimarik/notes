#include "DroidBlaster.hpp"
#include "Log.hpp"

#include <unistd.h>

namespace dbs {
    DroidBlaster::DroidBlaster() {
        packt::Log::info("Creating DroidBlaster");
    }

    DroidBlaster::~DroidBlaster() {
        packt::Log::info("Destructing DroidBlaster");
    }


    packt::status DroidBlaster::onActivate() {
        packt::Log::info("Activating DroidBlaster");
        return packt::STATUS_OK;
    }

    void DroidBlaster::onDeactivate() {
        packt::Log::info("Deactivating DroidBlaster");
    }

    packt::status DroidBlaster::onStep() {
        packt::Log::info("Starting step");
        usleep(300000);
        packt::Log::info("Stepping done");
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
