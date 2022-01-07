#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H


#include <QVector3D>

#include "src/aabbcollider.h"
#include "src/component.h"
#include "src/cubecollider.h"


class RigidBodyComponent : public Component {
    /*** ATTRIBUTES ***/
public:
    static QVector3D gravity;
    static float bounceTreshold;
    static float frictionTreshold;

    QVector3D _momentum;
    float _weight;
    float _drag;
    float _bounciness;
    float _friction;

    Transform* _previousTransform = new Transform(GetParent());

    /*** METHODS ***/
public:
    RigidBodyComponent(float weight, float drag, float bounciness, float friction, GameObject* parent);
    virtual ~RigidBodyComponent();

    void FixedUpdate(float delta) override;

    void Collision(Collider* other) override;

    void ResolveAABBAABB(AABBCollider* mine, AABBCollider* other);
};

#endif // RIGIDBODYCOMPONENT_H
