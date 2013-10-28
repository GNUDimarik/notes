#ifndef _DBS_ASTEROID_HPP_
#define _DBS_ASTEROID_HPP_

#include "Context.hpp"
#include "GraphicsService.hpp"
#include "GraphicsObject.hpp"
#include "PhysicsService.hpp"
#include "PhysicsObject.hpp"
#include "Types.hpp"

#include <boost/shared_ptr.hpp>
#include <vector>

namespace dbs {
    class Asteroid {
    public:
        typedef boost::shared_ptr<Asteroid> ptr;
        typedef std::vector<ptr> vec;
        typedef vec::iterator vec_it;

    public:
        Asteroid(packt::Context* pContext);

        void spawn();
        void update();

    private:
        packt::GraphicsService* mGraphicsService;
        packt::TimeService* mTimeService;

        packt::GraphicsObject::ptr mMesh;
        packt::PhysicsObject::ptr mPhysics;
    };
}
#endif
