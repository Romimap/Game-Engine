#include "inputmanager.h"


bool InputManager::captureMouse = false;
std::map<int, bool> InputManager::pressed;
std::map<int, bool> InputManager::active;
std::map<int, bool> InputManager::released;
QVector2D InputManager::previous;
QVector2D InputManager::current;
QVector2D InputManager::delta;


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

void InputManager::SampleMousePosition() {
    if (captureMouse) {
        previous = QVector2D(100, 100);
        current = QVector2D(QCursor::pos().x(), QCursor::pos().y());
        Engine::Singleton->cursor().setPos(100, 100);
    } else {
        previous = current;
        current = QVector2D(QCursor::pos().x(), QCursor::pos().y());
    }
    delta = current - previous;
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

int InputManager::MouseDX() {
    return delta.x();
}

int InputManager::MouseDY() {
    return delta.y();
}

QVector2D InputManager::MousePosition() {
    return current;
}

void InputManager::SetCaptureMouse(bool capture) {
    captureMouse = capture;
}

InputManager::InputManager() {

}
