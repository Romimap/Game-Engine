#include "transform.h"

Transform::Transform (GameObject* gameObject) : _gameObject(gameObject) {
    _positionMatrix.setToIdentity();
    _rotationMatrix.setToIdentity();
    _scaleMatrix.setToIdentity();
}

Transform::Transform (Transform* transform, GameObject* gameObject) : _gameObject(gameObject) {
    _positionMatrix = transform->_positionMatrix;
    _rotationMatrix = transform->_rotationMatrix;
    _scaleMatrix = transform->_scaleMatrix;
}

QVector3D Transform::Forward() {
    return _rotationMatrix * QVector3D(0, 0, -1);
}

QVector3D Transform::Up() {
    return _rotationMatrix * QVector3D(0, -1, 0);
}

QVector3D Transform::Right() {
    return _rotationMatrix * QVector3D(1, 0, 0);
}

void Transform::Translate (QVector3D t) {
    _positionMatrix.translate(t);
}

void Transform::GlobalTranslate (QVector3D t) {
    t = GlobalTransformMatrix() * t; //Not sure about that
    Translate(t);
}

void Transform::SetPosition (QVector3D t) {
    _positionMatrix.setToIdentity();
    Translate(t);
}

void Transform::SetPosition (float x, float y, float z) {
    SetPosition(QVector3D(x, y, z));
}

QVector3D Transform::GetPosition() {
    float m30 = _positionMatrix.column(3).x();
    float m31 = _positionMatrix.column(3).y();
    float m32 = _positionMatrix.column(3).z();

    return QVector3D(m30, m31, m32);
}

QVector3D Transform::GetGlobalPosition() {
    QMatrix4x4 globalMatrix = GlobalTransformMatrix();

    return (globalMatrix * QVector4D(0, 0, 0, 1)).toVector3D();
}

void Transform::Rotate (QQuaternion t) {
    _rotationMatrix.rotate(t);
}

void Transform::RotateAround (float t, QVector3D a) {
    QMatrix4x4 r;
    r.setToIdentity();
    r.rotate(t, a);
    _rotationMatrix = r * _rotationMatrix;
}

void Transform::SetRotation (QQuaternion t) {
    _rotationMatrix.setToIdentity();
    _rotationMatrix.rotate(t);
}

void Transform::SetRotation (float x, float y, float z) {
    SetRotation(QQuaternion::fromEulerAngles(x, y, z));
}

QQuaternion Transform::GetRotation() {
    float m00 = _rotationMatrix.column(0).x();
    float m01 = _rotationMatrix.column(0).y();
    float m02 = _rotationMatrix.column(0).z();
    float m10 = _rotationMatrix.column(1).x();
    float m11 = _rotationMatrix.column(1).y();
    float m12 = _rotationMatrix.column(1).z();
    float m20 = _rotationMatrix.column(2).x();
    float m21 = _rotationMatrix.column(2).y();
    float m22 = _rotationMatrix.column(2).z();

    float tr = m00 + m11 + m22;
    float qw, qx, qy, qz;

    if (tr > 0) {
      float S = sqrt(tr+1.0) * 2; // S=4*qw
      qw = 0.25 * S;
      qx = (m21 - m12) / S;
      qy = (m02 - m20) / S;
      qz = (m10 - m01) / S;
    } else if ((m00 > m11)&(m00 > m22)) {
      float S = sqrt(1.0 + m00 - m11 - m22) * 2; // S=4*qx
      qw = (m21 - m12) / S;
      qx = 0.25 * S;
      qy = (m01 + m10) / S;
      qz = (m02 + m20) / S;
    } else if (m11 > m22) {
      float S = sqrt(1.0 + m11 - m00 - m22) * 2; // S=4*qy
      qw = (m02 - m20) / S;
      qx = (m01 + m10) / S;
      qy = 0.25 * S;
      qz = (m12 + m21) / S;
    } else {
      float S = sqrt(1.0 + m22 - m00 - m11) * 2; // S=4*qz
      qw = (m10 - m01) / S;
      qx = (m02 + m20) / S;
      qy = (m12 + m21) / S;
      qz = 0.25 * S;
    }

    return QQuaternion(qw, qx, qy, qz);
}

void Transform::Scale (QVector3D t) {
    _scaleMatrix.scale(t);
}

void Transform::Scale (float x, float y, float z) {
    Scale(QVector3D(x, y, z));
}

void Transform::SetScale (QVector3D t) {
    _scaleMatrix.setToIdentity();
    Scale(t);
}

void Transform::SetScale (float x, float y, float z) {
    SetScale(QVector3D(x, y, z));
}

QMatrix4x4 Transform::LocalTransformMatrix () {
    return _positionMatrix * _rotationMatrix * _scaleMatrix;
}

QMatrix4x4 Transform::GlobalTransformMatrix () {
    QMatrix4x4 transform = LocalTransformMatrix();

    if (_gameObject->GetParent() == nullptr) {
        return transform;
    }
    return _gameObject->GetParent()->GetTransform()->GlobalTransformMatrix() * transform;
}
