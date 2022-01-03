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

#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>
#include <QWidget>
#include <QTimer>

#include "gameobject.h"
#include "engine.h"
#include "glmesh.h"
#include "material.h"
#include "components/playercontrollercomponent.h"
#include "components/rigidbodycomponent.h"
#include "components/rotatorcomponent.h"
#include "components/octreerenderercomponent.h"
#include "components/meshrenderercomponent.h"
#include "worldgenerator.h"



#include "cubecollider.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("Voxel Game Engine");
    app.setApplicationVersion("0.1");

    GameObject root;
    root.NAME = "root";

    Camera camera(&root);
    camera.NAME = "camera";
    camera.GetTransform()->SetPosition(0, 270, 0);
    camera.GetTransform()->SetRotation(0, 0, 0);
    camera.GetTransform()->SetScale(1, 1, 1);

    Engine engine;
    engine.show();


    //ORIGIN (OFFSET 0 260 0)
    GLMesh* cubeMesh = new GLMesh("../Game-Engine/misc/Cube.obj");

    GameObject cube(&root);
    cube.GetTransform()->SetRotation(0, 0, 0);
    cube.GetTransform()->SetPosition(0, 260, 0);
    Material* cubeMat = new Material(":/meshvshader.glsl", ":/meshfshader.glsl");
    cubeMat->SetSlot2D(":/default.png", 0);
    MeshRendererComponent cubeMeshRenderer(cubeMesh, cubeMat, &cube);

    GameObject X(&root);
    X.GetTransform()->SetRotation(0, 0, 0);
    X.GetTransform()->SetPosition(4, 260, 0);
    Material* XMat = new Material(":/meshvshader.glsl", ":/meshfshader.glsl");
    XMat->SetSlot2D(":/r.png", 0);
    MeshRendererComponent XMeshRenderer(cubeMesh, XMat, &X);

    GameObject Y(&root);
    Y.GetTransform()->SetRotation(0, 0, 0);
    Y.GetTransform()->SetPosition(0, 264, 0);
    Material* YMat = new Material(":/meshvshader.glsl", ":/meshfshader.glsl");
    YMat->SetSlot2D(":/g.png", 0);
    MeshRendererComponent YMeshRenderer(cubeMesh, YMat, &Y);

    GameObject Z(&root);
    Z.GetTransform()->SetRotation(0, 0, 0);
    Z.GetTransform()->SetPosition(0, 260, 4);
    Material* ZMat = new Material(":/meshvshader.glsl", ":/meshfshader.glsl");
    ZMat->SetSlot2D(":/b.png", 0);
    MeshRendererComponent ZMeshRenderer(cubeMesh, ZMat, &Z);


    //CHUNK
    GameObject octree(&root);
    octree.GetTransform()->SetRotation(0, 0, 0);
    octree.GetTransform()->SetPosition(0, 0, 0);
    OctreeRendererComponent octreeRenderer(&octree);




    //Material* skyboxMat = new Material(nullptr, ":/vshader.glsl", ":/skyboxfshader.glsl");
    //
    //RenderData skyboxRenderData(skyboxMat, plane);
    //
    //GameObject skybox(&camera);
    //skybox.GetTransform()->SetRotation(90, 0, 0);
    //skybox.GetTransform()->SetPosition(0, 0, -0.75);
    //skybox.SetRenderData(&skyboxRenderData);

    PlayerControllerComponent playerController(32, 0.1, nullptr, &camera);

    // Currently for testing purposes
    WorldGenerator worldGenerator("New world", 123456u);

    return app.exec();
}
