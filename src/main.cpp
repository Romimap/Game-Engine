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

#include "cubecollider.h"

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
    camera.GetTransform()->SetPosition(0, 0, -4);
    camera.GetTransform()->SetRotation(0, 0, 0);
    camera.GetTransform()->SetScale(1, 1, 1);

    Engine engine;
    engine.show();

    GameObject monkeyA(&root);
    monkeyA.NAME = "MonkeyA";
    monkeyA.GetTransform()->SetPosition(-1, 0, 0);
    monkeyA.GetTransform()->SetRotation(0, 0, 0);
    monkeyA.GetTransform()->SetScale(1, 1, 1);


    GameObject monkeyBParentParent(&root);
    monkeyBParentParent.NAME = "MonkeyBParent";
    monkeyBParentParent.GetTransform()->SetPosition(0, 0, 0);
    monkeyBParentParent.GetTransform()->SetRotation(350, -380, 90);
    monkeyBParentParent.GetTransform()->SetScale(1, 1, 1);

    GameObject monkeyBParent(&monkeyBParentParent);
    monkeyBParent.NAME = "MonkeyBParent";
    monkeyBParent.GetTransform()->SetPosition(0, 0, 0);
    monkeyBParent.GetTransform()->SetRotation(40, 40, 40);
    monkeyBParent.GetTransform()->SetScale(1, 1, 1);

    GameObject monkeyB(&root);
    monkeyB.NAME = "MonkeyB";
    monkeyB.GetTransform()->SetPosition(1, 0, 0);
    monkeyB.GetTransform()->SetRotation(40, 40, 40);
    monkeyB.GetTransform()->SetScale(1, 1, 1);

    CubeCollider* cubeColliderMonkeyA = new CubeCollider(2, 2, 2);
    monkeyA.SetCollider(cubeColliderMonkeyA);
    CubeCollider* cubeColliderMonkeyB = new CubeCollider(2, 2, 2);
    monkeyB.SetCollider(cubeColliderMonkeyB);

    PlayerControllerComponent playerController(&monkeyA);
    RigidBodyComponent rigidBodyComponent(1, &monkeyB);

    RenderData renderDataMonkey(new Material(), new GLMesh("/home/romimap/Documents/git/QT/misc/Cube.obj"));
    monkeyA.SetRenderData(&renderDataMonkey);
    monkeyB.SetRenderData(&renderDataMonkey);

    return app.exec();
}