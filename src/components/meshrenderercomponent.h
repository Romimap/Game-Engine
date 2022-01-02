#ifndef MESHRENDERERCOMPONENT_H
#define MESHRENDERERCOMPONENT_H

#include <src/component.h>
#include <src/material.h>
#include <src/glmesh.h>
#include <src/camera.h>

class MeshRendererComponent : public Component {
private:
    GLMesh* _mesh;
    Material* _material;
public:
    MeshRendererComponent(GLMesh* mesh, Material* material, GameObject* parent);
    void Draw() override;
};

#endif // MESHRENDERERCOMPONENT_H
