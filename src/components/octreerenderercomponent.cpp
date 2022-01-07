#include "octreerenderercomponent.h"
#include "src/components/terrain/terraincomponent.h"

int OctreeRendererComponent::s_notUpToDateChunks = 0;

OctreeRendererComponent::OctreeRendererComponent(GameObject* parent) : Component(parent) {
    this->_name = "OctreeRendererComponent";

    _material = new Material(":/octreevshader.glsl", ":/octreefshader.glsl");

    _mesh = new GLMesh("../Game-Engine/misc/chunk.obj");

    TerrainComponent *TC = (TerrainComponent*)parent->GetDerivedComponent<TerrainComponent>();

    for (int y = 255; y < 256; y++) {
        for (int x = 0; x < 64; x++) {
            for (int z = 0; z < 64; z++) {
                unsigned char c = TC->getVoxelType(x, y, z, 0);
                cout << (int)c << " ";
            }
            cout << endl;
        }
        cout << endl;
        cout << endl;
        cout << endl;
    }

    char* data4 = (char*)malloc(4*16*4 * sizeof (char));
    for (int k = 0; k < 4*16*4; k++) {
        data4[k] = 0;
        //data4[k] = (rand() % 5 == 0) * 255;;
    }

    char* data16 = (char*)malloc(16*64*16 * sizeof (char));
    for (int k = 0; k < 16*64*16; k++) {
        data16[k] = 0;
        //data16[k] = (rand() % 5 == 0) * 255;;
    }

    int ymin = 1000;
    int ymax = -999;
    char* data64 = (char*)malloc(64*256*64 * sizeof (char));
    for (int k = 0; k < 64*256*64; k++) {
        int x =  k % 64;
        int y = (k / 64) % 256;
        int z = (k / 64) / 256;

        ymin = std::min(y, ymin);
        ymax = std::max(y, ymax);

        int x16 = x / 4;
        int y16 = y / 4;
        int z16 = z / 4;
        int k16 = x16 + 16*y16 + 16*64*z16;

        if (k16 >= 16*64*16) {
            printf("ERROR 16 k=%d x=%d y=%d z=%d\n", k16, x16, y16, z16);
            exit(0);
        }

        int x4 = x16 / 4;
        int y4 = y16 / 4;
        int z4 = z16 / 4;
        int k4 = x4 + 4*y4 + 4*16*z4;

        //printf("64 k=%d x=%d y=%d z=%d\n", k, x, y, z);
        //printf("16 k=%d x=%d y=%d z=%d\n", k16, x16, y16, z16);
        //printf("4  k=%d x=%d y=%d z=%d\n\n", k4, x4, y4, z4);


        if (k4 >= 4*16*4) {
            printf("ERROR 4 k=%d x=%d y=%d z=%d\n", k4, x4, y4, z4);
            exit(0);
        }

        //if (x == 0 || y == 0 || z == 0) {
        //unsigned char c = TC->getVoxelType(x, y, z, 0);
        //if (c) {
        if (x == 0 && y == 0 && z == 0) {
            data64[k] = 1;
            data16[k16] = 1;
            data4[k4] = 1;
        } else {
            data64[k] = 0;
        }
        //data1024[k] = 0b11111111;
        //data64[k] = (rand() % 20 == 0) * 255;
    }

    cout << "ymin = " << ymin << ", ymax = " << ymax << endl;

    _material->SetSlot3D(0, 4, 16, 4, data4);
    _material->SetSlot3D(1, 16, 64, 16, data16);
    _material->SetSlot3D(2, 64, 256, 64, data64);
    _material->SetSlot2D(":/Textures.png", 3);
}


void OctreeRendererComponent::Draw() {
    _material->_TexSlot0->bind(0);
    _material->_TexSlot1->bind(1);
    _material->_TexSlot2->bind(2);
    _material->_TexSlot3->bind(3);
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
    _material->program.setUniformValue("colorlod4", 0);
    _material->program.setUniformValue("colorlod16", 1);
    _material->program.setUniformValue("colorlod64", 2);
    _material->program.setUniformValue("materials", 3);

    // Draw Mesh
    _mesh->draw(&_material->program);
}

void OctreeRendererComponent::Update(float delta) {
    if (_layer4.empty() && _layer16.empty() && _layer64.empty()) return;

    QOpenGLFunctions_3_1 glfunc;
    glfunc.initializeOpenGLFunctions();

    int quota = std::max(1, OCTREE_RENDERER_VOXELS_FRAME_LIMIT/s_notUpToDateChunks); //Quota with a min value of 1

    while (!_layer4.empty() && quota > 0) {
        OctreeRendererChange* change = _layer4.pop_front();
        change->_value;
        _material->_TexSlot0->bind();
        glfunc.glTexSubImage3D  (GL_TEXTURE_3D, 0, change->_x, change->_y, change->_z, 1, 1, 1, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, &change->_value);
        delete change;

        quota--;
    }

    while (!_layer16.empty() && quota > 0) {
        OctreeRendererChange* change = _layer16.pop_front();
        change->_value;
        _material->_TexSlot1->bind();
        glfunc.glTexSubImage3D(GL_TEXTURE_3D, 0, change->_x, change->_y, change->_z, 1, 1, 1, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, &change->_value);
        delete change;

        quota--;
    }

    while (!_layer64.empty() && quota > 0) {
        OctreeRendererChange* change = _layer64.pop_front();
        change->_value;
        _material->_TexSlot2->bind();
        glfunc.glTexSubImage3D(GL_TEXTURE_3D, 0, change->_x, change->_y, change->_z, 1, 1, 1, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, &change->_value);
        delete change;

        quota--;
    }

    if (_layer4.empty() && _layer16.empty() && _layer64.empty()) s_notUpToDateChunks--;
}

void OctreeRendererComponent::ApplyChanges(LinkedQueue<OctreeRendererChange> &layer4, LinkedQueue<OctreeRendererChange> &layer16, LinkedQueue<OctreeRendererChange> &layer64) {
    if (_layer4.empty() && _layer16.empty() && _layer64.empty()
    && (!layer4.empty() || !layer16.empty() || !layer64.empty())) {
        s_notUpToDateChunks++;
    }

    _layer4.push_back_clear(layer4);
    _layer16.push_back_clear(layer16);
    _layer64.push_back_clear(layer64);
}
