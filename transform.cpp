#include "transform.h"
#include "QTransform"


Transform::Transform() {
    _position = QVector3D(0, 0, 0);
    _rotation = QQuaternion::fromEulerAngles(0, 0, 0);
    _scale = QVector3D(0, 0, 0);
}

Transform::Transform(QVector3D position, QQuaternion rotation, QVector3D scale) :
    _position(position),
    _rotation(rotation),
    _scale(scale) {

}

QMatrix4x4 Transform::WorldMatrix() {
    QTransform pos;
}


