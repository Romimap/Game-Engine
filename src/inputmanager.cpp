#include "inputmanager.h"

std::map<int, bool> InputManager::pressed;
std::map<int, bool> InputManager::active;
std::map<int, bool> InputManager::released;

void InputManager::Press(int keyCode) {
    if (KeyDown(keyCode)) return;

    pressed.insert(std::pair<int, bool>(keyCode, true));
    active.insert(std::pair<int, bool>(keyCode, true));
    released.erase(keyCode);
}

void InputManager::Resease(int keyCode) {
    if (KeyUp(keyCode)) return;

    released.insert(std::pair<int, bool>(keyCode, true));
    active.erase(keyCode);
    pressed.erase(keyCode);
}

void InputManager::NextFrame() {
    pressed.clear();
    released.clear();
}

bool InputManager::Key(int keyCode) {
    return active.count(keyCode);
}

bool InputManager::KeyDown(int keyCode) {
    return pressed.count(keyCode);
}

bool InputManager::KeyUp(int keyCode) {
    return released.count(keyCode);
}

InputManager::InputManager()
{

}
