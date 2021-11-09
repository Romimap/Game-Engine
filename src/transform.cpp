#include "transform.h"

//CONSTRUCTORS
Transform::Transform(GameObject* gameObject) {
    _position = QVector3D(0, 0, 0);
    _rotation = QQuaternion(0, 0, 0, 1);
    _scale = QVector3D(1, 1, 1);
    _gameObject = gameObject;
}

Transform::Transform(QVector3D position, QQuaternion rotation, QVector3D scale, GameObject* gameObject) :
    _position(position),
    _rotation(rotation),
    _scale(scale),
    _gameObject(gameObject) {

}

//GETTERS / SETTERS
QVector3D Transform::GetPosition () {
    return _position;
}

void Transform::SetPosition (QVector3D position) {
    _position = position;
}

void Transform::SetPosition (float x, float y, float z) {
    _position = QVector3D(x, y, z);
}

QQuaternion Transform::GetRotation () {
    return _rotation;
}

void Transform::SetRotation (QQuaternion rotation) {
    _rotation = rotation;
}

void Transform::SetRotation (QVector3D eulerRotation) {
    _rotation = QQuaternion::fromEulerAngles(eulerRotation);
}

void Transform::SetRotation (float x, float y, float z) {
    _rotation = QQuaternion::fromEulerAngles(QVector3D(x, y, z));
}

QVector3D Transform::GetScale () {
    return _scale;
}

void Transform::SetScale (QVector3D scale) {
    _scale = scale;
}

void Transform::SetScale (float x, float y, float z) {
    _scale = QVector3D(x, y, z);
}

//METHODS
QMatrix4x4 Transform::LocalTransformMatrix () {
    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(_position);
    localMatrix.rotate(_rotation);
    localMatrix.scale(_scale);

    return localMatrix;
}

void Transform::Translate (QVector3D translation) {
    _position += translation;
}

void Transform::Translate (float x, float y, float z) {
    _position += QVector3D(x, y, z);
}

void Transform::Rotate (QQuaternion rotation) {
    _rotation = _rotation * rotation;
}

void Transform::Rotate (QVector3D eulerRotation) {
    QQuaternion rotation = QQuaternion::fromEulerAngles(eulerRotation);
    _rotation = _rotation * rotation;
}

void Transform::Rotate (float x, float y, float z) {
    QQuaternion rotation = QQuaternion::fromEulerAngles(QVector3D(x, y, z));
    _rotation = _rotation * rotation;
}

void Transform::Scale (QVector3D scale) {
    _scale *= scale;
}

void Transform::Scale (float x, float y, float z) {
    _scale *= QVector3D(x, y, z);
}

void Transform::Scale (float s) {
    _scale *= s;
}

QMatrix4x4 Transform::TransformMatrix () {
    QMatrix4x4 localMatrix = LocalTransformMatrix();

    GameObject* parent = _gameObject->GetParent();
    if (parent == nullptr) {
        return localMatrix;
    }

    return parent->GetTransform()->TransformMatrix() * localMatrix;
}

