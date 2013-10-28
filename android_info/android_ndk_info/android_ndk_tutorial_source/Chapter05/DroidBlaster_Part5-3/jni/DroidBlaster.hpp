#ifndef _PACKT_DROIDBLASTER_HPP_
#define _PACKT_DROIDBLASTER_HPP_

#include "ActivityHandler.hpp"
#include "Context.hpp"
#include "TimeService.hpp"
#include "Types.hpp"

#include <android_native_app_glue.h>

namespace dbs {
    class DroidBlaster : public packt::ActivityHandler {
    public:
        DroidBlaster(packt::Context& pContext,
                     android_app* pApplication);
        ~DroidBlaster();

    protected:
        packt::status onActivate();
        void onDeactivate();
        packt::status onStep();

        void onStart();
        void onResume();
        void onPause();
        void onStop();
        void onDestroy();

        void onSaveState(void** pData, size_t* pSize);
        void onConfigurationChanged();
        void onLowMemory();

        void onCreateWindow();
        void onDestroyWindow();
        void onGainFocus();
        void onLostFocus();

    private:
        void clear();
        void drawCursor(int pSize, int pX, int pY);

    private:
        android_app* mApplication;
        ANativeWindow_Buffer mWindowBuffer;
        packt::TimeService* mTimeService;

        bool mInitialized;

        float mPosX;
        float mPosY;
        const int32_t mSize;
        const float mSpeed;
    };
}
#endif
