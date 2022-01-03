#include "octreerenderercomponent.h"
#include "src/camera.h"
#include "src/gameobject.h"

OctreeRendererComponent::OctreeRendererComponent(GameObject* parent) : Component(parent) {
    _material = new Material(":/octreevshader.glsl", ":/octreefshader.glsl");
    _mesh = new GLMesh("../Game-Engine/misc/chunk.obj");

    //TODO, do that but with an octree

    QImage heightmap;
    heightmap.load(":/heightmap-1024x1024.png");

    _material->_TexSlot0 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    _material->_TexSlot0->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    _material->_TexSlot0->setWrapMode(QOpenGLTexture::Repeat);
    _material->_TexSlot0->create();
    _material->_TexSlot0->setSize(4, 32, 4);
    _material->_TexSlot0->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    _material->_TexSlot0->allocateStorage();
    char* data4 = (char*)malloc(4*32*4 * sizeof (char));
    for (int k = 0; k < 4*32*4; k++) {
        data4[k] = 0;
        //data4[k] = (rand() % 5 == 0) * 255;;
    }

    _material->_TexSlot1 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    _material->_TexSlot1->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    _material->_TexSlot1->setWrapMode(QOpenGLTexture::Repeat);
    _material->_TexSlot1->create();
    _material->_TexSlot1->setSize(16, 128, 16);
    _material->_TexSlot1->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    _material->_TexSlot1->allocateStorage();
    char* data16 = (char*)malloc(16*128*16 * sizeof (char));
    for (int k = 0; k < 16*128*16; k++) {
        data16[k] = 0;
        //data16[k] = (rand() % 5 == 0) * 255;;
    }


    _material->_TexSlot2 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    _material->_TexSlot2->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    _material->_TexSlot2->setWrapMode(QOpenGLTexture::Repeat);
    _material->_TexSlot2->create();
    _material->_TexSlot2->setSize(64, 512, 64);
    _material->_TexSlot2->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    _material->_TexSlot2->allocateStorage();
    char* data64 = (char*)malloc(64*512*64 * sizeof (char));
    for (int k = 0; k < 64*512*64; k++) {
        int x =  k % 64;
        int y = (k / 64) % 512;
        int z = (k / 64) / 512;


        int x16 = x / 4;
        int y16 = y / 4;
        int z16 = z / 4;
        int k16 = x16 + 16*y16 + 16*128*z16;

        if (k16 >= 16*128*16) {
            printf("ERROR 16 k=%d x=%d y=%d z=%d\n", k16, x16, y16, z16);
            exit(0);
        }

        int x4 = x16 / 4;
        int y4 = y16 / 4;
        int z4 = z16 / 4;
        int k4 = x4 + 4*y4 + 4*32*z4;

        //printf("64 k=%d x=%d y=%d z=%d\n", k, x, y, z);
        //printf("16 k=%d x=%d y=%d z=%d\n", k16, x16, y16, z16);
        //printf("4  k=%d x=%d y=%d z=%d\n\n", k4, x4, y4, z4);


        if (k4 >= 4*32*4) {
            printf("ERROR 4 k=%d x=%d y=%d z=%d\n", k4, x4, y4, z4);
            exit(0);
        }

        //QColor c = heightmap.pixelColor(x * 16, z * 16);

        //if (c.red() / 16 + 32 < y) {
        if (x == 0 || y == 0 || z == 0) {
            data64[k] = 0b11111111;
            data16[k16] = 0b11111111;
            data4[k4] = 0b11111111;
        } else {
            data64[k] = 0;
        }
        //data1024[k] = 0b11111111;
        //data64[k] = (rand() % 20 == 0) * 255;

        if (k == 64*512*64 - 1) {
            printf("64 k=%d x=%d y=%d z=%d\n", k, x, y, z);
            printf("16 k=%d x=%d y=%d z=%d\n", k16, x16, y16, z16);
            printf("4  k=%d x=%d y=%d z=%d\n\n", k4, x4, y4, z4);
        }
    }

    _material->_TexSlot0->setData(QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, data4);
    _material->_TexSlot1->setData(QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, data16);
    _material->_TexSlot2->setData(QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, data64);
}


void OctreeRendererComponent::Draw() {
    _material->_TexSlot0->bind(0);
    _material->_TexSlot1->bind(1);
    _material->_TexSlot2->bind(2);
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

    // Draw Mesh
    _mesh->draw(&_material->program);
}
