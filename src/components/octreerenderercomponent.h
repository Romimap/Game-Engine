#ifndef OCTREERENDERERCOMPONENT_H
#define OCTREERENDERERCOMPONENT_H


#include "src/camera.h"
#include "src/component.h"
#include "src/gameobject.h"
#include "src/glmesh.h"
#include "src/material.h"


class OctreeRendererComponent : Component {
private:
    Material* _material;
    GLMesh* _mesh;

public:
    OctreeRendererComponent(GameObject* parent);

    void Draw() override;
};

#endif // OCTREERENDERERCOMPONENT_H
