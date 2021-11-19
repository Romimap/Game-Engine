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


#include "cubecollider.h"

//Hacky thing to make sure that no two meshes are the same, should do something about it
#define CUBE_MESH cubeMesh
#define SPHERE_MESH sphereMesh

#define RENDER_DATA_CUBE &renderDataCube
#define RENDER_DATA_SPHERE &renderDataShpere
#define RENDER_DATA_UNLITSPHERE &renderDataShpereUnlit

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("TP1 et 2");
    app.setApplicationVersion("0.1");

    GameObject root;
    root.NAME = "root";

    Camera camera(&root);
    camera.NAME = "camera";
    camera.GetTransform()->SetPosition(3, 7, 3);
    camera.GetTransform()->SetRotation(-50, 10, 0);
    camera.GetTransform()->SetScale(1, 1, 1);

    Engine engine;
    engine.show();

    Material* defaultMat = new Material(nullptr, nullptr, ":/vshader.glsl", ":/fshader.glsl");
    Material* unlitMat = new Material(nullptr, nullptr, ":/vshader.glsl", ":/fshaderunlit.glsl");
    Material* rMat = new Material(":/r.png", nullptr, ":/vshader.glsl", ":/fshader.glsl");
    Material* gMat = new Material(":/g.png", nullptr, ":/vshader.glsl", ":/fshader.glsl");
    Material* bMat = new Material(":/b.png", nullptr, ":/vshader.glsl", ":/fshader.glsl");

    GLMesh* cubeMesh = new GLMesh("/home/romimap/Documents/git/QT/misc/Cube.obj", "/home/romimap/Documents/git/QT/misc/Cube.obj");
    GLMesh* sphereMesh = new GLMesh("/home/romimap/Documents/git/QT/misc/Sphere.obj", "/home/romimap/Documents/git/QT/misc/Cube.obj");

    RenderData renderDataCube(defaultMat, CUBE_MESH);
    RenderData renderDataShpere(defaultMat, SPHERE_MESH);
    RenderData renderDataShpereUnlit(unlitMat, SPHERE_MESH);
    RenderData rCube(rMat, CUBE_MESH);
    RenderData gCube(gMat, CUBE_MESH);
    RenderData bCube(bMat, CUBE_MESH);


    //
    //ORIGIN
    //
    GameObject rx(&root);
    rx.GetTransform()->SetPosition(1, 0, 0);
    rx.GetTransform()->SetScale(0.3, 0.15, 0.15);
    rx.SetRenderData(&rCube);

    GameObject gy(&root);
    gy.GetTransform()->SetPosition(0, 1, 0);
    gy.GetTransform()->SetScale(0.15, 0.3, 0.15);
    gy.SetRenderData(&gCube);

    GameObject bz(&root);
    bz.GetTransform()->SetPosition(0, 0, 1);
    bz.GetTransform()->SetScale(0.15, 0.15, 0.3);
    bz.SetRenderData(&bCube);


    //
    //SUN EARTH MOON
    //
    GameObject SolarSystem(&root);
    SolarSystem.NAME = "SolarSystem";
    SolarSystem.GetTransform()->SetPosition(0, 10, 0);
    SolarSystem.GetTransform()->Scale(1, 1, 1);

    GameObject Sun(&SolarSystem);
    Sun.NAME = "Sun";
    Sun.GetTransform()->SetPosition(0, 0, 0);
    Sun.GetTransform()->Scale(1.5, 1.5, 1.5);
    Sun.SetRenderData(RENDER_DATA_UNLITSPHERE);
    RotatorComponent sunRotator(&Sun, 0, 10, 0);


    GameObject EarthMoonOrbit(&SolarSystem);
    EarthMoonOrbit.NAME = "EarthMoonOrbit";
    EarthMoonOrbit.GetTransform()->SetPosition(0, 0, 0);
    EarthMoonOrbit.GetTransform()->SetRotation(0, 0, 25);
    RotatorComponent earthMoonOrbitRotator(&EarthMoonOrbit, 0, 25, 0);

    GameObject Barycenter(&EarthMoonOrbit);
    Barycenter.NAME = "Barycenter";
    Barycenter.GetTransform()->SetPosition(5, 0, 0);
    RotatorComponent barycenterRotator(&Barycenter, 0, 45, 0);

    GameObject EarthCenter(&Barycenter);
    EarthCenter.NAME = "EarthCenter";
    EarthCenter.GetTransform()->SetPosition(-0.4641, 0, 0);
    EarthCenter.GetTransform()->SetRotation(0, 0, 40);
    //EarthCenter.SetRenderData(&renderDataShpere);
    RotatorComponent earthCenterRotator(&EarthCenter, 0, -70, 0);

    GameObject Earth(&EarthCenter);
    Earth.NAME = "Earth";
    RotatorComponent earthRotator (&Earth, 0, 90, 0);
    Earth.SetRenderData(RENDER_DATA_SPHERE);

    GameObject Moon(&Barycenter);
    Moon.NAME = "Moon";
    Moon.GetTransform()->SetPosition(2, 0, 0);
    Moon.GetTransform()->SetRotation(0, 0, 15);
    Moon.GetTransform()->SetScale(0.3, 0.3, 0.3);
    Moon.SetRenderData(RENDER_DATA_SPHERE);

    //
    //COLLISIONS
    //
    GameObject Floor(&root);
    Floor.NAME = "Floor";
    Floor.GetTransform()->SetPosition(0, 0, 0);
    Floor.GetTransform()->SetRotation(0, 0, 0);
    Floor.GetTransform()->SetScale(5, 0.1, 5);
    AABBCollider* aabbColliderFloor = new AABBCollider(2, 2, 2);
    Floor.SetCollider(aabbColliderFloor);
    Floor.SetRenderData(RENDER_DATA_CUBE);

    GameObject WallA(&root);
    WallA.NAME = "WallA";
    WallA.GetTransform()->SetPosition(-5, 5, 0);
    WallA.GetTransform()->SetRotation(0, 0, 0);
    WallA.GetTransform()->SetScale(0.1, 5, 5);
    AABBCollider* aabbColliderWallA = new AABBCollider(2, 2, 2);
    WallA.SetCollider(aabbColliderWallA);
    WallA.SetRenderData(RENDER_DATA_CUBE);

    GameObject WallB(&root);
    WallB.NAME = "WallB";
    WallB.GetTransform()->SetPosition(0, 5, -5);
    WallB.GetTransform()->SetRotation(0, 0, 0);
    WallB.GetTransform()->SetScale(5, 5, 0.1);
    AABBCollider* aabbColliderWallB = new AABBCollider(2, 2, 2);
    WallB.SetCollider(aabbColliderWallB);
    WallB.SetRenderData(RENDER_DATA_CUBE);

    GameObject PhysicCube(&root);
    PhysicCube.NAME = "PhysicCube";
    PhysicCube.GetTransform()->SetPosition(7, 10, 10);
    PhysicCube.GetTransform()->SetRotation(0, 0, 0);
    PhysicCube.GetTransform()->SetScale(1, 1, 1);
    AABBCollider* aabbColliderPhysicCube = new AABBCollider(2, 2, 2);
    PhysicCube.SetCollider(aabbColliderPhysicCube);
    RigidBodyComponent rigidBodyComponent(1, 0.001, 0.2, 0.001, &PhysicCube);
    rigidBodyComponent._momentum.setX(-12);
    rigidBodyComponent._momentum.setY(3);
    rigidBodyComponent._momentum.setZ(-8);
    PhysicCube.SetRenderData(RENDER_DATA_CUBE);

    PlayerControllerComponent cameraPlayerController(5, 0.1, &rigidBodyComponent, &camera);


    return app.exec();
}
