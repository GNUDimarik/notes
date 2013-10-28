#ifndef _PACKT_DROIDBLASTER_HPP_
#define _PACKT_DROIDBLASTER_HPP_

#include "ActivityHandler.hpp"
#include "Types.hpp"

namespace dbs {
    struct DroidBlasterSaveState {
        int32_t mActivationCount;
    };

    class DroidBlaster : public packt::ActivityHandler {
    public:
        DroidBlaster(void* pState);
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
        int32_t mActivationCount;
    };
}
#endif
