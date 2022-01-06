#include "terrainrenderercomponent.h"

TerrainRendererComponent::TerrainRendererComponent(GameObject* parent) : Component(parent) {
    this->_name = "TerrainRendererComponent";
}

void TerrainRendererComponent::Draw() {

    /** Bind the 3D textures to the shader **/

    _materiald3D->bindTexture(2, 0);
    _materiald3D->bindTexture(1, 1);
    _materiald3D->bindTexture(0, 2);
    _materiald3D->_program.bind();

    /** Get the MVP matrices **/

    //NOTE: for some reason the camera matrix needs to be inverted
    QMatrix4x4 cameraMatrix = Camera::ActiveCamera->GetTransform()->GlobalTransformMatrix();
    QMatrix4x4 viewMatrix = Camera::ActiveCamera->GetTransform()->ViewMatrix();
    QMatrix4x4 modelMatrix = GetParent()->GetTransform()->GlobalTransformMatrix();

    /** Send matrices and useful data to the shader **/

    _materiald3D->_program.setUniformValue("projection_matrix", Camera::ActiveCamera->_projectionMatrix);
    _materiald3D->_program.setUniformValue("view_matrix", viewMatrix);
    _materiald3D->_program.setUniformValue("model_matrix", modelMatrix);
    _materiald3D->_program.setUniformValue("inv_projection_matrix", Camera::ActiveCamera->_projectionMatrix.inverted());
    _materiald3D->_program.setUniformValue("inv_view_matrix", viewMatrix.inverted());
    _materiald3D->_program.setUniformValue("inv_model_matrix", modelMatrix.inverted());
    _materiald3D->_program.setUniformValue("camera_matrix", cameraMatrix);

    _materiald3D->_program.setUniformValue("screen_size", Camera::ActiveCamera->ScreenSize());
    _materiald3D->_program.setUniformValue("screen_ratios", Camera::ActiveCamera->GetScreenRatios());

    /** Associate texture units **/

    _materiald3D->_program.setUniformValue("colorlod4", 0);
    _materiald3D->_program.setUniformValue("colorlod16", 1);
    _materiald3D->_program.setUniformValue("colorlod64", 2);
}
