#ifndef STRUCTS_H
#define STRUCTS_H

#include <QVector3D>

class GameObject;
struct RayCastHit {
    float _distance;
    QVector3D _position;
    GameObject* _gameobject;

    RayCastHit() : _distance(-1), _position(QVector3D()), _gameobject(nullptr) {

    }
};


#endif // STRUCTS_H
