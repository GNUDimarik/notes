#include "PhysicsTarget.hpp"
#include "Log.hpp"

namespace packt {
    PhysicsTarget::PhysicsTarget(b2World* pWorld, b2Body* pBodyObj,
        Location& pTarget, float pFactor):
      mFactor(pFactor), mTarget(pTarget) {
        b2BodyDef lEmptyBodyDef;
        b2Body* lEmptyBody = pWorld->CreateBody(&lEmptyBodyDef);

        b2MouseJointDef lMouseJointDef;
        lMouseJointDef.bodyA = lEmptyBody;
        lMouseJointDef.bodyB = pBodyObj;
        lMouseJointDef.target = b2Vec2(0.0f, 0.0f);
        lMouseJointDef.maxForce = 50.0f * pBodyObj->GetMass();
        lMouseJointDef.dampingRatio = 1.0f;
        lMouseJointDef.frequencyHz = 3.5f;
        mMouseJoint = (b2MouseJoint*)
            pWorld->CreateJoint(&lMouseJointDef);
    }

    void PhysicsTarget::setTarget(float pX, float pY) {
        b2Vec2 lTarget((mTarget.mPosX + pX * mFactor) / SCALE_FACTOR,
            (mTarget.mPosY + pY * mFactor) / SCALE_FACTOR);
        mMouseJoint->SetTarget(lTarget);
    }
}
