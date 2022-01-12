/*
    SKYBOXRENDERERCOMPONENT.H
    Draws a Skybox
*/

#ifndef SKYBOXRENDERERCOMPONENT_H
#define SKYBOXRENDERERCOMPONENT_H

#include "src/component.h"
#include "src/glmesh.h"
#include "src/material.h"

class SkyboxRendererComponent : Component {
    GLMesh* _mesh;
    Material* _material;
public:
    SkyboxRendererComponent(GameObject* parent);
    void Draw() override;
};

#endif // SKYBOXRENDERERCOMPONENT_H
