#include "Ship.hpp"
#include "Log.hpp"

namespace dbs {
    Ship::Ship(packt::Context* pContext) :
      mGraphicsService(pContext->mGraphicsService),
      mLocation(), mAnimSpeed(8.0f) {
        mSprite = pContext->mGraphicsService->registerSprite(
            mGraphicsService->registerTexture("ship.png"), 64, 64,
            &mLocation);
    }

    void Ship::spawn() {
        const int32_t FRAME_1 = 0; const int32_t FRAME_NB = 8;
        mSprite->setAnimation(FRAME_1, FRAME_NB, mAnimSpeed, true);
        mLocation.setPosition(mGraphicsService->getWidth() * 1 / 2,
                              mGraphicsService->getHeight() * 1 / 4);
    }
}
