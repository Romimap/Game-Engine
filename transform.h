#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class Transform{
    //MEMBERS
private:
    QVector3D _position;
    QQuaternion _rotation;
    QVector3D _scale;

    //CONSTRUCTORS
public:
    Transform();
    Transform(QVector3D position, QQuaternion rotation, QVector3D scale);

    //METHODS
public:
    void Translate (QVector3D translation);
    void Rotate (QVector3D eulerRotation);
    void Scale (QVector3D scale);
    QMatrix4x4 WorldMatrix ();
};

#endif // TRANSFORM_H
