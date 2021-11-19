#ifndef CUBECOLLIDER_H
#define CUBECOLLIDER_H

#include "collider.h"
#include "QVector3D"
#include "gameobject.h"

class CubeCollider : public Collider {
private:
    QVector3D _size;
public:
    CubeCollider(float x = 1, float y = 1, float z = 1);
    void SetAABB() override;
    GameObject* GetParent() override {return _parent;}
    float x() {return _size.x();}
    float y() {return _size.y();}
    float z() {return _size.z();}

    bool Collision(Collider* other) override;


};

#endif // CUBECOLLIDER_H
