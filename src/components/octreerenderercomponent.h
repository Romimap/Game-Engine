#ifndef OCTREERENDERERCOMPONENT_H
#define OCTREERENDERERCOMPONENT_H


#include <QVector3D>

#include "src/camera.h"
#include "src/component.h"
#include "src/gameobject.h"
#include "src/glmesh.h"
#include "src/linkedqueue.h"
#include "src/material.h"

#include "src/components/terrain/octreecomponent.h"


#define OCTREE_RENDERER_VOXELS_FRAME_LIMIT 60


struct OctreeRendererChange {
    int _x;
    int _y;
    int _z;
    unsigned char _value;

    OctreeRendererChange(int x, int y, int z, unsigned char value) : _x(x), _y(y), _z(z), _value(value){}
};


class OctreeRendererComponent : Component {
    /*** ATTRIBUTES ***/
private:
    Material* _material;
    GLMesh* _mesh;

    LinkedQueue<OctreeRendererChange> _layer4;
    LinkedQueue<OctreeRendererChange> _layer16;
    LinkedQueue<OctreeRendererChange> _layer64;

    static int s_notUpToDateChunks;

    /*** METHODS ***/
public:
    OctreeRendererComponent(GameObject* parent);

    void Start() override;
    void Draw() override;
    void Update(float delta) override;
    void ApplyChanges(LinkedQueue<OctreeRendererChange> &layer4, LinkedQueue<OctreeRendererChange> &layer16, LinkedQueue<OctreeRendererChange> &layer64);
};

#endif // OCTREERENDERERCOMPONENT_H
