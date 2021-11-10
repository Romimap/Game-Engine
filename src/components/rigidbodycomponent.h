#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <src/component.h>
#include <QVector3D>

class RigidBodyComponent : public Component {
public:
    static QVector3D gravity;

private:
    QVector3D _momentum;
    float _weight;
    Transform* previousTransform = new Transform(GetParent());
public:
    RigidBodyComponent(float weight, GameObject* parent);

    void Update(float delta) override {
        delete previousTransform;
        previousTransform = new Transform(GetParent());

        GetParent()->GetTransform()->GlobalTranslate(_momentum * delta);

        _momentum += gravity * delta;
    }

    void Collision(CollisionData* collisionData) override {

        //_momentum = QVector3D(0, 0, 0);
    }
};

#endif // RIGIDBODYCOMPONENT_H
