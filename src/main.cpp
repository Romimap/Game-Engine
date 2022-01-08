/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <chrono>
#include <iostream>

#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>
#include <QWidget>
#include <QTimer>

#include "cubecollider.h"
#include "engine.h"
#include "gameobject.h"
#include "glmesh.h"
#include "linkedqueue.h"
#include "material.h"

#include "components/playercontrollercomponent.h"
#include "components/rigidbodycomponent.h"
#include "components/rotatorcomponent.h"
#include "components/octreerenderercomponent.h"
#include "components/meshrenderercomponent.h"
#include "components/worldgeneratorcomponent.h"
#include "components/skyboxrenderercomponent.h"


int main(int argc, char *argv[]) {

    /** Create the application **/

    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("Voxel Game Engine");
    app.setApplicationVersion("0.1");

    /** Create the root GameObject and the Camera with a controller **/

    GameObject root("Root");

    Camera* camera = new Camera("Camera", 2, &root);
    camera->GetTransform()->SetPosition(32, 160, 32);
    camera->GetTransform()->SetRotation(45, 0, 0);
    camera->GetTransform()->SetScale(1, 1, 1);

    new PlayerControllerComponent(12, 0.1, nullptr, camera);

    /** Create the Engine **/

    Engine engine(1280, 720);
    engine.show();

    /** Add reference objects to the scene **/

    GLMesh* cubeMesh = new GLMesh("../Game-Engine/misc/Cube.obj");

    GameObject* cube = new GameObject("White cube", &root);
    cube->GetTransform()->SetRotation(0, 0, 0);
    cube->GetTransform()->SetPosition(0, 0, 0);
    Material* cubeMat = new Material(":/meshvshader.glsl", ":/meshfshader.glsl");
    cubeMat->SetSlot2D(":/default.png", 0);
    new MeshRendererComponent(cubeMesh, cubeMat, cube);
    //cube->SetCollider(new AABBCollider(1, 1, 1));

    GameObject* X = new GameObject("Red cube", &root);
    X->GetTransform()->SetRotation(0, 0, 0);
    X->GetTransform()->SetPosition(4, 0, 0);
    Material* XMat = new Material(":/meshvshader.glsl", ":/meshfshader.glsl");
    XMat->SetSlot2D(":/r.png", 0);
    new MeshRendererComponent(cubeMesh, XMat, X);

    GameObject* Y = new GameObject("Green cube", &root);
    Y->GetTransform()->SetRotation(0, 0, 0);
    Y->GetTransform()->SetPosition(0, 4, 0);
    Material* YMat = new Material(":/meshvshader.glsl", ":/meshfshader.glsl");
    YMat->SetSlot2D(":/g.png", 0);
    new MeshRendererComponent(cubeMesh, YMat, Y);

    GameObject* Z = new GameObject("Blue cube", &root);
    Z->GetTransform()->SetRotation(0, 0, 0);
    Z->GetTransform()->SetPosition(0, 0, 4);
    Material* ZMat = new Material(":/meshvshader.glsl", ":/meshfshader.glsl");
    ZMat->SetSlot2D(":/b.png", 0);
    new MeshRendererComponent(cubeMesh, ZMat, Z);

    /** Create a world generator GameObject **/

    std::cout << "World gen starts at: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;

    GameObject* worldGenerator = new GameObject("World generator", &root);
    new WorldGeneratorComponent("New world", TerrainType::PERLIN_2D, worldGenerator);


    //CHUNK
    /*** OCTREE ***/
    // GameObject* octree = new GameObject("Octree", &root);
    // octree->GetTransform()->SetRotation(0, 0, 0);
    // octree->GetTransform()->SetPosition(0, 0, 0);
    // OctreeRendererComponent* octreeRenderer = new OctreeRendererComponent(octree);

    // LinkedQueue<OctreeRendererChange> changes4;

    // for (int i = 0; i < 1000; i++) {
    //     OctreeRendererChange* change = new OctreeRendererChange(rand() % 4, rand() % 16, rand() % 4, rand() % 2);
    //     changes4.push_back(change);
    // }

    // LinkedQueue<OctreeRendererChange> changes16;
    // LinkedQueue<OctreeRendererChange> changes64;

    // octreeRenderer->ApplyChanges(changes4, changes16, changes64);


    GameObject *skybox = new GameObject("Skybox", camera);
    skybox->GetTransform()->SetRotation(90, 0, 0);
    skybox->GetTransform()->SetPosition(0, 0, -0.75);
    new SkyboxRendererComponent(skybox);

    return app.exec();
}
