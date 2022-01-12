/*
    MESHRENDERERCOMPONENT.H
    Renders a mesh using a material.
*/
#ifndef MESHRENDERERCOMPONENT_H
#define MESHRENDERERCOMPONENT_H


#include <src/camera.h>
#include <src/component.h>
#include <src/glmesh.h>
#include <src/material.h>


class MeshRendererComponent : public Component {
    /*** ATTRIBUTES ***/
private:
    Material* _material;
    GLMesh* _mesh;

    /*** METHODS ***/
public:
    MeshRendererComponent(GLMesh* mesh, Material* material, GameObject* parent);

    void Draw() override;
};

#endif // MESHRENDERERCOMPONENT_H
