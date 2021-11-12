#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <src/component.h>
#include <QVector3D>
#include "src/cubecollider.h"

class RigidBodyComponent : public Component {
public:
    static QVector3D gravity;
    static float speedTreshold;

private:
    QVector3D _momentum;
    float _weight;
    Transform* _previousTransform = new Transform(GetParent());
public:
    RigidBodyComponent(float weight, GameObject* parent);

    void Update(float delta) override {
        delete _previousTransform;
        _previousTransform = new Transform(GetParent()->GetTransform(), GetParent());

        GetParent()->GetTransform()->GlobalTranslate(_momentum * delta);

        _momentum += gravity * delta;
    }

    void Collision(CollisionData* collisionData) override {
        //If we are barely moving and a collision is detected, the momentum is set to 0 and we retrieve our previous transform
        //Strange cases could appear (2 box not moving mid-air, a box stuck on a wall or ceiling...)
        if (_momentum.lengthSquared() < speedTreshold) {
            _momentum = QVector3D();
            GetParent()->GetTransform()->Lerp(_previousTransform, 1);
            return;
        }

        //Initialisation
        Collider* a = collisionData->a->GetCollider();
        Collider* b = collisionData->b->GetCollider();

        CollisionData data = *collisionData;
        Transform* latestTransform = new Transform(GetParent()->GetTransform(), GetParent());

        //Estimate the collision time
        GetParent()->GetTransform()->Lerp(_previousTransform, 0.5);
        for (int i = 0; i < 4; i++) {
            data = a->Collision(b);
            if (data.collision)
                GetParent()->GetTransform()->Lerp(_previousTransform, 0.5);
            else
                GetParent()->GetTransform()->Lerp(latestTransform, 0.5);
        }
        delete latestTransform;

        //Resolve Collision
        if (typeid (*a) == typeid (CubeCollider) && typeid (*b) == typeid (CubeCollider))
            ResolveCollision((CubeCollider*)a, (CubeCollider*)b, collisionData);

        //TODO: Apply Force


    }

    void ResolveCollision (CubeCollider* a, CubeCollider* b, CollisionData* collisionData) {
        // Not sure if this is true, but the closest point from a cube to another
        // should be on the 3 faces incident to the vertex that is the closest to the other cube
        // We the would have to check :
        // - Vertex to Vertex   (1 * 1 tests)
        // - Vertex to Edge     (1 * 3 tests)
        // - Vertex to Face     (1 * 3 tests)
        // - Edge to Vertex     (3 * 1 tests)
        // - Edge to Edge       (3 * 3 tests)
        // - Face to Vertex     (1 * 3 tests)
        // And take the pair of point that have the minimum distance between them
        // We should make 22 tests

    }
};

#endif // RIGIDBODYCOMPONENT_H
