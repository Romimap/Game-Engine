QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

CONFIG += c++17

SOURCES += src/main.cpp \
    src/chunk.cpp \
    src/aabb.cpp \
    src/aabbcollider.cpp \
    src/camera.cpp \
    src/component.cpp \
    src/components/meshrenderercomponent.cpp \
    src/components/octreerenderercomponent.cpp \
    src/components/playercontrollercomponent.cpp \
    src/components/rigidbodycomponent.cpp \
    src/engine.cpp \
    src/gameobject.cpp \
    src/glmesh.cpp \
    src/inputmanager.cpp \
    src/material.cpp \
    src/components/rotatorcomponent.cpp \
    src/transform.cpp \
    src/worldgenerator.cpp

HEADERS += \
    src/chunk.h \
    src/aabb.h \
    src/aabbcollider.h \
    src/camera.h \
    src/collider.h \
    src/component.h \
    src/components/meshrenderercomponent.h \
    src/components/octreerenderercomponent.h \
    src/components/playercontrollercomponent.h \
    src/components/rigidbodycomponent.h \
    src/consts.h \
    src/engine.h \
    src/gameobject.h \
    src/glmesh.h \
    src/inputmanager.h \
    src/material.h \
    src/components/rotatorcomponent.h \
    src/transform.h \
    libraries/octree/array.h \
    libraries/octree/array2d.h \
    libraries/octree/numtraits.h \
    libraries/octree/octree.h \
    libraries/octree/octree.tcc \
    libraries/octree/point3d.h \
    libraries/octree/shareddata.h \
    libraries/octree/tinyvector.h \
    libraries/PerlinNoise/PerlinNoise.hpp \
    src/worldgenerator.h

RESOURCES += \
    shaders/shaders.qrc \
    textures/textures.qrc

# install
target.path = $$[YOUR_PATH]
INSTALLS += target

DISTFILES += \
    misc/Suzanne.obj\
    misc/Cube.obj \
    misc/plane.obj
