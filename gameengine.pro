QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += main.cpp \
    component.cpp \
    gameobject.cpp \
    transform.cpp

SOURCES += \
    geometryengine.cpp \
    terrainwidget.cpp

HEADERS += \
    component.h \
    gameobject.h \
    geometryengine.h \
    terrainwidget.h \
    transform.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[YOUR_PATH]
INSTALLS += target
