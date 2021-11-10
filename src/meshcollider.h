#ifndef MESHCOLLIDER_H
#define MESHCOLLIDER_H

#include "collider.h"

#include <vector>
#include <QVector3D>

class MeshCollider : Collider {
private:
    std::vector<QVector3D*>* _vertices;
    std::vector<int>* _indices;

public:
    MeshCollider(char* path);
    ~MeshCollider() override {}

public:
    void SetAABB () override;
    std::vector<std::string> split (std::string s, std::string delimiter);
};

#endif // MESHCOLLIDER_H
