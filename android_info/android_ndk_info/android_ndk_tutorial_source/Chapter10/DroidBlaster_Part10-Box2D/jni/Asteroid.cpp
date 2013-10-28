#include "Asteroid.hpp"
#include "Log.hpp"

namespace dbs {
    Asteroid::Asteroid(packt::Context* pContext) :
      mTimeService(pContext->mTimeService),
      mGraphicsService(pContext->mGraphicsService) {
        mPhysics = pContext->mPhysicsService->registerEntity(
            0X1, 0x2, 64, 1.0f);
        mSprite = pContext->mGraphicsService->registerSprite(
            mGraphicsService->registerTexture(
                    "/sdcard/droidblaster/asteroid.png"),
            64, 64, &mPhysics->mLocation);
    }

    void Asteroid::spawn() {
        const float MIN_VELOCITY = 1.0f, VELOCITY_RANGE = 19.0f;
        const float MIN_ANIM_SPEED = 8.0f, ANIM_SPEED_RANGE = 16.0f;

        float lVelocity = -(RAND(VELOCITY_RANGE) + MIN_VELOCITY);
        float lPosX = RAND(mGraphicsService->getWidth());
        float lPosY = RAND(mGraphicsService->getHeight())
                      + mGraphicsService->getHeight();
        mPhysics->initialize(lPosX, lPosY, 0.0f, lVelocity);

        float lAnimSpeed = MIN_ANIM_SPEED + RAND(ANIM_SPEED_RANGE);
        mSprite->setAnimation(8, -1, lAnimSpeed, true);
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
