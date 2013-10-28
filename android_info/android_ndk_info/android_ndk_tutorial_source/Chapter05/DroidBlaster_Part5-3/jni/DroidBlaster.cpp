#include "DroidBlaster.hpp"
#include "Log.hpp"

#include <math.h>

namespace dbs {
    DroidBlaster::DroidBlaster(packt::Context& pContext,
                               android_app* pApplication) :
        mApplication(pApplication),
        mTimeService(pContext.mTimeService),
        mInitialized(false),
        mPosX(0), mPosY(0), mSize(24), mSpeed(100.0f) {
        packt::Log::info("Creating DroidBlaster");
    }

    DroidBlaster::~DroidBlaster() {
        packt::Log::info("Destructing DroidBlaster");
    }


    packt::status DroidBlaster::onActivate() {
        packt::Log::info("Activating DroidBlaster");

        mTimeService->reset();

        // Forces 32 bits format.
        ANativeWindow* lWindow = mApplication->window;
        if (ANativeWindow_setBuffersGeometry(lWindow, 0, 0,
            WINDOW_FORMAT_RGBX_8888) < 0) {
            return packt::STATUS_KO;
        }

        // Needs to lock the window buffer to get its properties.
        if (ANativeWindow_lock(lWindow, &mWindowBuffer, NULL) >= 0) {
            ANativeWindow_unlockAndPost(lWindow);
        } else {
            return packt::STATUS_KO;
        }

        // Position the mark in the center.
        if (!mInitialized) {
            mPosX = mWindowBuffer.width / 2;
            mPosY = mWindowBuffer.height / 2;
            mInitialized = true;
        }
        return packt::STATUS_OK;
    }

    void DroidBlaster::onDeactivate() {
        packt::Log::info("Deactivating DroidBlaster");
    }

    packt::status DroidBlaster::onStep() {
        mTimeService->update();

        // Moves the mark at 100 pixels per second.
        mPosX = fmod(mPosX + mSpeed * mTimeService->elapsed(),
                     mWindowBuffer.width);

        // Locks the window buffer and draws on it.
        ANativeWindow* lWindow = mApplication->window;
        if (ANativeWindow_lock(lWindow, &mWindowBuffer, NULL) >= 0) {
            clear();
            drawCursor(mSize, mPosX, mPosY);
            ANativeWindow_unlockAndPost(lWindow);
            return packt::STATUS_OK;
        } else {
            return packt::STATUS_KO;
        }
    }

    void DroidBlaster::clear() {
        memset(mWindowBuffer.bits, 0, mWindowBuffer.stride
                   * mWindowBuffer.height * sizeof(uint32_t*));
    }

    void DroidBlaster::drawCursor(int pSize, int pX, int pY) {
        const int lHalfSize = pSize / 2;

        const int lUpLeftX = pX - lHalfSize;
        const int lUpLeftY = pY - lHalfSize;
        const int lDownRightX = pX + lHalfSize;
        const int lDownRightY = pY + lHalfSize;

        // Goes through each line.
        uint32_t* lLine = ((uint32_t*)mWindowBuffer.bits)
                         + (mWindowBuffer.stride * lUpLeftY);
        for (int iY = lUpLeftY; iY <= lDownRightY; iY++) {
            // Goes through each pixel to fill in the line.
            for (int iX = lUpLeftX; iX <= lDownRightX; iX++) {
                lLine[iX] = 255;
            }
            // Goes to the next line of pixels.
            lLine = lLine + mWindowBuffer.stride;
        }
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
