QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += main.cpp \
    aabb.cpp \
    camera.cpp \
    component.cpp \
    engine.cpp \
    gameobject.cpp \
    glmesh.cpp \
    material.cpp \
    meshcollider.cpp \
    rotatorcomponent.cpp \
    transform.cpp

HEADERS += \
    aabb.h \
    camera.h \
    collider.h \
    component.h \
    engine.h \
    gameobject.h \
    glmesh.h \
    material.h \
    meshcollider.h \
    rotatorcomponent.h \
    transform.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[YOUR_PATH]
INSTALLS += target

DISTFILES += \
    Suzanne.obj
