#include "meshrenderercomponent.h"

///Creates a MeshRendererComponent with <mesh> as a mesh, and <material> as material.
MeshRendererComponent::MeshRendererComponent(GLMesh* mesh, Material* material, GameObject* parent) : Component(parent), _mesh(mesh), _material(material) {
    this->_name = "MeshRendererComponent";
}

///Draws the mesh (_mesh) with the material (_material)
void MeshRendererComponent::Draw() {
    _material->_TexSlot0->bind(0);
    _material->program.bind();

    //NOTE: for some reason the camera matrix needs to be inverted
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


    // Use texture units
    _material->program.setUniformValue("color", 0);

    // Draw Mesh
    _mesh->draw(&_material->program);
}
