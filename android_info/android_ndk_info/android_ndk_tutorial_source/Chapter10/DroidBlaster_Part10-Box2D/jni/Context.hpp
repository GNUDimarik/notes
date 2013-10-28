#ifndef _PACKT_CONTEXT_HPP_
#define _PACKT_CONTEXT_HPP_

#include "Types.hpp"

namespace packt {
    class GraphicsService;
    class InputService;
    class PhysicsService;
    class SoundService;
    class TimeService;

    struct Context {
        GraphicsService* mGraphicsService;
        InputService*    mInputService;
        PhysicsService*  mPhysicsService;
        SoundService*    mSoundService;
        TimeService*     mTimeService;
    };
}
#endif
