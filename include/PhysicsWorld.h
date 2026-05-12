#pragma once
#include <vector>
#include "RigidBody.h"

class PhysicsWorld {
public:
    void step(float dt);
    RigidBody* addBody(const RigidBodyDef& def);

private:
    std::vector<RigidBody> m_bodies;
};
