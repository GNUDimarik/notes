#ifndef PACKT_PHYSICSTARGET_HPP
#define PACKT_PHYSICSTARGET_HPP

#include "Types.hpp"
#include <boost/smart_ptr.hpp>
#include <Box2D/Box2D.h>

namespace packt {
    class PhysicsTarget {
    public:
        typedef boost::shared_ptr<PhysicsTarget> ptr;

    public:
        PhysicsTarget(b2World* pWorld, b2Body* pBodyObj,
            Location& pTarget, float pFactor);
        void setTarget(float pX, float pY);

    private:
        b2MouseJoint* mMouseJoint;
        float mFactor; Location& mTarget;
    };
}
#endif
