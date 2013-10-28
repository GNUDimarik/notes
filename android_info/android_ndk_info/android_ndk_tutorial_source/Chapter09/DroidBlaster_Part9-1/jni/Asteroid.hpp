#ifndef _DBS_ASTEROID_HPP_
#define _DBS_ASTEROID_HPP_

#include "Context.hpp"
#include "GraphicsService.hpp"
#include "GraphicsSprite.hpp"
#include "Types.hpp"

namespace dbs {
    class Asteroid {
    public:
        Asteroid(packt::Context* pContext);

        void spawn();
        void update();

    private:
        packt::GraphicsService* mGraphicsService;
        packt::TimeService* mTimeService;

        packt::GraphicsSprite* mSprite;
        packt::Location mLocation;
        float mSpeed;
    };
}
#endif
