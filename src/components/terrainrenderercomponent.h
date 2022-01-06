#ifndef TERRAINRENDERERCOMPONENT_H
#define TERRAINRENDERERCOMPONENT_H


#include <iostream>

#include "src/camera.h"
#include "src/component.h"
#include "src/gameobject.h"
#include "src/material3d.h"

#include "src/components/terrain/terraincomponent.h"


using namespace std;


class TerrainRendererComponent : Component {
    /*** ATTRIBUTES ***/
    Material3D* _materiald3D = nullptr;

    /*** METHODS ***/
public:
    /** CONSTRUCTORS/DESTRUCTORS **/
    TerrainRendererComponent(GameObject* parent);

    /** GENERIC COMPONENT'S METHODS **/
    void Start() override;
    void Draw() override;
};

#endif // TERRAINRENDERERCOMPONENT_H
