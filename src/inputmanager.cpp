#include "inputmanager.h"


bool InputManager::captureMouse = false;
std::map<int, bool> InputManager::pressed;
std::map<int, bool> InputManager::active;
std::map<int, bool> InputManager::released;
QVector2D InputManager::previous;
QVector2D InputManager::current;
QVector2D InputManager::delta;


///Called when the user presses a key.
void InputManager::Press(int keyCode) {
    if (KeyDown(keyCode)) return;

    pressed.insert(std::pair<int, bool>(keyCode, true));
    active.insert(std::pair<int, bool>(keyCode, true));
    released.erase(keyCode);
}

///Called when the user releases a key
void InputManager::Resease(int keyCode) {
    if (KeyUp(keyCode)) return;

    released.insert(std::pair<int, bool>(keyCode, true));
    active.erase(keyCode);
    pressed.erase(keyCode);
}

///Samples the mouse position every frames
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

///Called by the engine to set internal states.
void InputManager::NextFrame() {
    pressed.clear();
    released.clear();
}

///Checks if the key <keyCode> is actually pressed
bool InputManager::Key(int keyCode) {
    return active.count(keyCode);
}

///Checks if the key <keyCode> just got pressed
bool InputManager::KeyDown(int keyCode) {
    return pressed.count(keyCode);
}

///Checks if the key <keyCode> just got released
bool InputManager::KeyUp(int keyCode) {
    return released.count(keyCode);
}

///Gets the mouse change in position on the X axis from the last frame
int InputManager::MouseDX() {
    return delta.x();
}

///Gets the mouse change in position on the Y axis from the last frame
int InputManager::MouseDY() {
    return delta.y();
}

///Gets the mouse position
QVector2D InputManager::MousePosition() {
    return current;
}

///Captures/Releases the mouse, usefull for FPS controls
void InputManager::SetCaptureMouse(bool capture) {
    captureMouse = capture;
}

InputManager::InputManager() {

}
