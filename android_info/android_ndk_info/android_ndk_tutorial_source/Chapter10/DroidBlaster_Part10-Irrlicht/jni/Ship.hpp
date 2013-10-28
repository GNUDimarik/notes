#ifndef _DBS_SHIP_HPP_
#define _DBS_SHIP_HPP_

#include "Context.hpp"
#include "InputService.hpp"
#include "GraphicsService.hpp"
#include "GraphicsObject.hpp"
#include "PhysicsService.hpp"
#include "PhysicsObject.hpp"
#include "PhysicsTarget.hpp"
#include "TimeService.hpp"
#include "Types.hpp"

namespace dbs {
    class Ship {
    public:
        Ship(packt::Context* pContext);

        void spawn();
        void update();

    private:
        packt::GraphicsService* mGraphicsService;
        packt::InputService* mInputService;
        packt::TimeService* mTimeService;

        packt::GraphicsObject::ptr mMesh;
        packt::PhysicsObject::ptr mPhysics;
        packt::PhysicsTarget::ptr mTarget;
    };
}
#endif
