QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += src/main.cpp \
    src/aabb.cpp \
    src/camera.cpp \
    src/component.cpp \
    src/engine.cpp \
    src/gameobject.cpp \
    src/glmesh.cpp \
    src/cubecollider.cpp \
    src/octreedata.cpp \
    src/material.cpp \
    src/meshcollider.cpp \
    src/octreecollider.cpp \
    src/octreegameobject.cpp \
    src/rotatorcomponent.cpp \
    src/transform.cpp

HEADERS += \
    src/aabb.h \
    src/camera.h \
    src/collider.h \
    src/component.h \
    src/cubecollider.h \
    src/engine.h \
    src/gameobject.h \
    src/glmesh.h \
    src/material.h \
    src/meshcollider.h \
    src/octreedata.h \
    src/octreegameobject.h \
    src/rotatorcomponent.h \
    src/octreecollider.h \
    src/transform.h \
    libraries/octree/array.h \
    libraries/octree/array2d.h \
    libraries/octree/numtraits.h \
    libraries/octree/octree.h \
    libraries/octree/octree.tcc \
    libraries/octree/point3d.h \
    libraries/octree/shareddata.h \
    libraries/octree/tinyvector.h

RESOURCES += \
    shaders/shaders.qrc \
    textures/textures.qrc

# install
target.path = $$[YOUR_PATH]
INSTALLS += target

DISTFILES += \
    Suzanne.obj
