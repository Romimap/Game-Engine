#include "skyboxrenderercomponent.h"
#include "src/camera.h"

SkyboxRendererComponent::SkyboxRendererComponent(GameObject* parent) : Component(parent) {
    _material = new Material(":/skyboxvshader.glsl", ":/skyboxfshader.glsl");
    _mesh = new GLMesh("../Game-Engine/misc/plane.obj");
}

///Draws a skybox with the depth buffer set to 1 (bellow everything)
void SkyboxRendererComponent::Draw() {
    _material->program.bind();

    QMatrix4x4 cameraMatrix = Camera::ActiveCamera->GetTransform()->GlobalTransformMatrix();
    QMatrix4x4 viewMatrix = Camera::ActiveCamera->GetTransform()->ViewMatrix();
    QMatrix4x4 modelMatrix = GetParent()->GetTransform()->GlobalTransformMatrix();

    // Set modelview-projection matrix
    _material->program.setUniformValue("projection_matrix", Camera::ActiveCamera->_projectionMatrix);
    _material->program.setUniformValue("view_matrix", viewMatrix);
    _material->program.setUniformValue("model_matrix", modelMatrix);
    _material->program.setUniformValue("inv_projection_matrix", Camera::ActiveCamera->_projectionMatrix.inverted());
    _material->program.setUniformValue("inv_view_matrix", viewMatrix.inverted());
    _material->program.setUniformValue("inv_model_matrix", modelMatrix.inverted());
    _material->program.setUniformValue("camera_matrix", cameraMatrix);

    _material->program.setUniformValue("screen_size", Camera::ActiveCamera->ScreenSize());
    _material->program.setUniformValue("screen_ratios", Camera::ActiveCamera->GetScreenRatios());

    // Draw Mesh
    _mesh->draw(&_material->program);
}
