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
    src/material.cpp \
    src/meshcollider.cpp \
    src/octreecollider.cpp \
    src/rotatorcomponent.cpp \
    src/transform.cpp

HEADERS += \
    src/header/aabb.h \
    src/header/camera.h \
    src/header/collider.h \
    src/header/component.h \
    src/header/engine.h \
    src/header/gameobject.h \
    src/header/glmesh.h \
    src/header/material.h \
    src/header/meshcollider.h \
    src/header/rotatorcomponent.h \
    src/header/octreecollider.h \
    src/header/transform.h \
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
