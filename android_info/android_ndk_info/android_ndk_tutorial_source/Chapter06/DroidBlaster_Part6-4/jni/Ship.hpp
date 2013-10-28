#ifndef _DBS_SHIP_HPP_
#define _DBS_SHIP_HPP_

#include "Context.hpp"
#include "GraphicsService.hpp"
#include "GraphicsSprite.hpp"
#include "Types.hpp"

namespace dbs {
    class Ship {
    public:
        Ship(packt::Context* pContext);

        void spawn();

    private:
        packt::GraphicsService* mGraphicsService;

        packt::GraphicsSprite* mSprite;
        packt::Location mLocation;
        float mAnimSpeed;
    };
}
#endif
