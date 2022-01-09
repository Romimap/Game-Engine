#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <cmath>

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>
#include <QVector4D>

#include "src/gameobject.h"


class GameObject;


class Transform {
    /*** ATTRIBUTES ***/
private:
    QMatrix4x4 _positionMatrix;
    QMatrix4x4 _rotationMatrix;
    QMatrix4x4 _scaleMatrix;

    GameObject* _gameObject;

    /*** METHODS ***/
public:
    Transform(GameObject* gameObject);
    Transform(Transform* transform, GameObject* gameObject);
    ~Transform() {

    }

    QVector3D Forward();
    QVector3D Up();
    QVector3D Left();

    void Translate(QVector3D t);
    void GlobalTranslate(QVector3D t);

    void SetPosition(QVector3D t);
    void SetPosition(float x, float y, float z);
    QVector3D GetPosition();
    QVector3D GetGlobalPosition();

    void Rotate(QQuaternion t);
    void RotateAround(float t, QVector3D a);
    void SetRotation(QQuaternion t);
    void SetRotation(float x, float y, float z);
    QQuaternion GetRotation();

    void Scale(QVector3D t);
    void Scale(float x, float y, float z);
    void SetScale(QVector3D t);
    void SetScale(float x, float y, float z);


    QMatrix4x4 LocalTransformMatrix();
    QMatrix4x4 GlobalTransformMatrix();
    QMatrix4x4 ViewMatrix();
};

#endif // TRANSFORM_H
