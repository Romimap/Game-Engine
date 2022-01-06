#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <QVector3D>
#include "src/cubecollider.h"
#include "src/component.h"
#include "src/aabbcollider.h"

class RigidBodyComponent : public Component {
public:
    static QVector3D gravity;
    static float bounceTreshold;
    static float frictionTreshold;

public:
    QVector3D _momentum;
    float _weight;
    float _drag;
    float _bounciness;
    float _friction;

    Transform* _previousTransform = new Transform(GetParent());
public:
    RigidBodyComponent(float weight, float drag, float bounciness, float friction, GameObject* parent);
    virtual ~RigidBodyComponent();

    void FixedUpdate(float delta) override;

    void Collision(Collider* other) override;

    void ResolveAABBAABB(AABBCollider* mine, AABBCollider* other);
};

#endif // RIGIDBODYCOMPONENT_H
