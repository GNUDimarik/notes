#include "Asteroid.hpp"
#include "Log.hpp"

namespace dbs {
    Asteroid::Asteroid(packt::Context* pContext) :
      mTimeService(pContext->mTimeService),
      mGraphicsService(pContext->mGraphicsService) {
        mPhysics = pContext->mPhysicsService->registerEntity(
            0X1, 0x2, 64, 1.0f);
        mMesh = pContext->mGraphicsService->registerObject(
            "rock.png", "asteroid.obj", &mPhysics->mLocation);
    }

    void Asteroid::spawn() {
        const float MIN_VELOCITY = 1.0f, VELOCITY_RANGE = 19.0f;
        const float MIN_SPIN_SPEED = 0.001f, SPIN_SPEED_RANGE = 3.0f;

        float lVelocity = -(RAND(VELOCITY_RANGE) + MIN_VELOCITY);
        float lPosX = RAND(mGraphicsService->getWidth());
        float lPosY = RAND(mGraphicsService->getHeight())
                      + mGraphicsService->getHeight();
        mPhysics->initialize(lPosX, lPosY, 0.0f, lVelocity);

        float lSpinSpeed = MIN_SPIN_SPEED + RAND(SPIN_SPEED_RANGE);
        mMesh->spin(0.0f, lSpinSpeed, 0.0f);
    }

    void Asteroid::update() {
        if ((mPhysics->mLocation.mPosX < 0.0f) ||
        (mPhysics->mLocation.mPosX > mGraphicsService->getWidth()) ||
        (mPhysics->mLocation.mPosY < 0.0f) ||
        (mPhysics->mLocation.mPosY >
                                mGraphicsService->getHeight() * 2)) {
            spawn();
        }
    }
}
