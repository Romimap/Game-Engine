#ifndef TERRAINRENDERERCOMPONENT_H
#define TERRAINRENDERERCOMPONENT_H


#include <iostream>

#include "src/component.h"


using namespace std;


class TerrainRendererComponent : Component {
    /*** ATTRIBUTES ***/

    /*** METHODS ***/
public:
    /** CONSTRUCTORS/DESTRUCTORS **/
    TerrainRendererComponent(GameObject* parent);

    /** GENERIC COMPONENT'S METHODS **/
    void Draw() override;
};

#endif // TERRAINRENDERERCOMPONENT_H
