#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <map>
#include <QApplication>


class InputManager
{
private:
    static std::map<int, bool> active;
    static std::map<int, bool> pressed;
    static std::map<int, bool> released;

public:
    InputManager();

    static void Press(int keyCode);

    static void Resease(int keyCode);

    static void NextFrame();

    static bool Key (int keyCode);

    static bool KeyDown (int keyCode);

    static bool KeyUp (int keyCode);
};

#endif // INPUTMANAGER_H
