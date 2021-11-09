#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "src/header/gameobject.h"

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class GameObject;
class Transform {
    //MEMBERS
private:
    QVector3D _position;
    QQuaternion _rotation;
    QVector3D _scale;
    GameObject* _gameObject;

    //CONSTRUCTORS
public:
    Transform(GameObject* gameObject);

    Transform(QVector3D position, QQuaternion rotation, QVector3D scale, GameObject* gameObject);

    //GETTERS / SETTERS
public:
    QVector3D GetPosition ();
    void SetPosition (QVector3D position);
    void SetPosition (float x, float y, float z);
    QQuaternion GetRotation ();
    void SetRotation (QQuaternion rotation);
    void SetRotation (QVector3D eulerRotation);
    void SetRotation (float x, float y, float z);
    QVector3D GetScale ();
    void SetScale (QVector3D scale);
    void SetScale (float x, float y, float z);

    //METHODS
private:
    QMatrix4x4 LocalTransformMatrix ();

public:
    void Translate (QVector3D translation);
    void Translate (float x, float y, float z);
    void Rotate (QQuaternion rotation);
    void Rotate (QVector3D eulerRotation);
    void Rotate (float x, float y, float z);
    void Scale (QVector3D scale);
    void Scale (float x, float y, float z);
    void Scale (float s);
    QMatrix4x4 TransformMatrix ();
};

#endif // TRANSFORM_H
