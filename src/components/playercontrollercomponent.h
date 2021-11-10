#ifndef PLAYERCONTROLLERCOMPONENT_H
#define PLAYERCONTROLLERCOMPONENT_H

#include "src/component.h"
#include <Qt>
#include "src/inputmanager.h"

class PlayerControllerComponent : public Component
{
public:
    PlayerControllerComponent(GameObject* parent);

    void Start() override {

    }

    void Update(float delta) override {
        float x = InputManager::Key('Q') - InputManager::Key('D');
        float z = InputManager::Key('Z') - InputManager::Key('S');

        GetParent()->GetTransform()->Translate(x * delta, 0, z * delta);
    }
};

#endif // PLAYERCONTROLLERCOMPONENT_H
