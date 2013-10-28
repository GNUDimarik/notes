#ifndef _PACKT_DROIDBLASTER_HPP_
#define _PACKT_DROIDBLASTER_HPP_

#include "ActivityHandler.hpp"
#include "Background.hpp"
#include "Context.hpp"
#include "GraphicsService.hpp"
#include "Ship.hpp"
#include "SoundService.hpp"
#include "TimeService.hpp"
#include "Types.hpp"

namespace dbs {
    class DroidBlaster : public packt::ActivityHandler {
    public:
        DroidBlaster(packt::Context* pContext);

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
        packt::GraphicsService* mGraphicsService;
        packt::SoundService*    mSoundService;
        packt::TimeService*     mTimeService;

        Background mBackground;
        Ship mShip;
    };
}
#endif
