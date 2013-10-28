#ifndef PACKT_PHYSICSSERVICE_HPP
#define PACKT_PHYSICSSERVICE_HPP

#include "PhysicsObject.hpp"
#include "TimeService.hpp"
#include "Types.hpp"

#include <Box2D/Box2D.h>

namespace packt {
    class PhysicsService : private b2ContactListener {
    public:
        PhysicsService(TimeService* pTimeService);

        PhysicsObject::ptr registerEntity(uint16 pCategory,
            uint16 pMask, int32_t pDiameter, float pRestitution);
        status update();

    private:
        void BeginContact(b2Contact* pContact);

    private:
        TimeService* mTimeService;
        PhysicsObject::vec mColliders;
        b2World mWorld;

        static const int32_t VELOCITY_ITER = 6;
        static const int32_t POSITION_ITER = 2;
    };
}
#endif
