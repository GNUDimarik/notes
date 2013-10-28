#include "Ship.hpp"
#include "Log.hpp"

namespace dbs {
    Ship::Ship(packt::Context* pContext) :
      mInputService(pContext->mInputService),
      mGraphicsService(pContext->mGraphicsService),
      mTimeService(pContext->mTimeService) {
        mPhysics = pContext->mPhysicsService->registerEntity(
            0x2, 0x1, 64, 0.0f);
        mTarget = mPhysics->createTarget(50.0f);
        mSprite = pContext->mGraphicsService->registerSprite(
            mGraphicsService->registerTexture(
                    "/sdcard/droidblaster/ship.png"),
            64, 64, &mPhysics->mLocation);
        mInputService->setRefPoint(&mPhysics->mLocation);
    }

    void Ship::spawn() {
        const int32_t FRAME_1 = 0; const int32_t FRAME_NB = 8;
        mSprite->setAnimation(FRAME_1, FRAME_NB, 8.0f, true);
        mPhysics->initialize(mGraphicsService->getWidth()  * 1 / 2,
            mGraphicsService->getHeight() * 1 / 4, 0.0f, 0.0f);
    }

    void Ship::update() {
        mTarget->setTarget(mInputService->getHorizontal(),
          mInputService->getVertical());
        if (mPhysics->mCollide) {
            packt::Log::info("Ship has been touched");
        }
    }
}
