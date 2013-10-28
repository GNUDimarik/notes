#ifndef PACKT_PHYSICSOBJECT_HPP
#define PACKT_PHYSICSOBJECT_HPP

#include "PhysicsTarget.hpp"
#include "Types.hpp"

#include <boost/smart_ptr.hpp>
#include <Box2D/Box2D.h>
#include <vector>

namespace packt {
    class PhysicsObject {
    public:
        typedef boost::shared_ptr<PhysicsObject> ptr;
        typedef std::vector<ptr> vec; typedef vec::iterator vec_it;

    public:
        PhysicsObject(uint16 pCategory, uint16 pMask,
            int32_t pDiameter, float pRestitution, b2World* pWorld);
        PhysicsTarget::ptr createTarget(float pFactor);

        void initialize(float pX, float pY,
            float pVelocityX, float pVelocityY);
        void update();

        bool mCollide;
        Location mLocation;

    private:
        b2World* mWorld;
        b2BodyDef mBodyDef; b2Body* mBodyObj;
        b2CircleShape mShapeDef; b2FixtureDef mFixtureDef;
    };
}
#endif
