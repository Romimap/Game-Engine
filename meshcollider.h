#ifndef MESHCOLLIDER_H
#define MESHCOLLIDER_H

#include "collider.h"
#include "aabb.h"

#include <vector>
#include <QVector3D>

class MeshCollider : Collider {
    std::vector<QVector3D*>* _vertices;
    std::vector<int>* _indices;

public:
    MeshCollider(char* path);

protected:
    AABB SetAABB () override;
};

#endif // MESHCOLLIDER_H
