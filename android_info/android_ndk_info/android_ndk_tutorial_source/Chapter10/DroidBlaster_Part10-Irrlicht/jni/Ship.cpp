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
        mMesh = pContext->mGraphicsService->registerObject(
            "metal.png", "ship.obj", &mPhysics->mLocation);
        mInputService->setRefPoint(&mPhysics->mLocation);
    }

    void Ship::spawn() {
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
