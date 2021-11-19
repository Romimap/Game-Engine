#include "cubecollider.h"
#include <xmmintrin.h>
#include <math.h>
#include <map>

#define projectToPlane(p, o, n) p-(QVector3D::dotProduct(n,p-o))*n

CubeCollider::CubeCollider(float x, float y, float z) : _size(QVector3D(x * 0.5, y * 0.5, z * 0.5)) {
    CubeCollider::SetAABB();
}

void CubeCollider::SetAABB() {
    _localAABB = AABB(-_size, _size);
}

//QVector3D projectToPlane (QVector3D& p, QVector3D& o, QVector3D& n) {
//    return p - (QVector3D::dotProduct(n, p - o)) * n;
//}

//TODO: implement separation by calculating overlaps, could check the axis with the smalest distance to the current plane
bool CubeCollider::Collision(Collider* other) {
    if (typeid(*other) == typeid(CubeCollider)) {

        CubeCollider* cubeOther = (CubeCollider*)other;

        QMatrix4x4 aTransform = _parent->GetTransform()->GlobalTransformMatrix();
        QMatrix4x4 bTransform = cubeOther->_parent->GetTransform()->GlobalTransformMatrix();

        QVector3D a1(  _size.x(),   _size.y(),   _size.z());
        QVector3D a2(- _size.x(),   _size.y(),   _size.z());
        QVector3D a3(  _size.x(), - _size.y(),   _size.z());
        QVector3D a4(- _size.x(), - _size.y(),   _size.z());
        QVector3D a5(  _size.x(),   _size.y(), - _size.z());
        QVector3D a6(- _size.x(),   _size.y(), - _size.z());
        QVector3D a7(  _size.x(), - _size.y(), - _size.z());
        QVector3D a8(- _size.x(), - _size.y(), - _size.z());

        a1 = aTransform * a1;
        a2 = aTransform * a2;
        a3 = aTransform * a3;
        a4 = aTransform * a4;
        a5 = aTransform * a5;
        a6 = aTransform * a6;
        a7 = aTransform * a7;
        a8 = aTransform * a8;

        std::vector<QVector3D*> aPath(8);
        aPath[0] = &a1;
        aPath[1] = &a2;
        aPath[2] = &a4;
        aPath[3] = &a8;
        aPath[4] = &a7;
        aPath[5] = &a5;
        aPath[6] = &a1;
        aPath[7] = &a2;

        std::vector<QVector3D*> aList(8);
        aList[0] = &a1;
        aList[1] = &a2;
        aList[2] = &a3;
        aList[3] = &a4;
        aList[4] = &a5;
        aList[5] = &a6;
        aList[6] = &a7;
        aList[7] = &a8;

        QVector3D b1(  cubeOther->_size.x(),   cubeOther->_size.y(),   cubeOther->_size.z());
        QVector3D b2(- cubeOther->_size.x(),   cubeOther->_size.y(),   cubeOther->_size.z());
        QVector3D b3(  cubeOther->_size.x(), - cubeOther->_size.y(),   cubeOther->_size.z());
        QVector3D b4(- cubeOther->_size.x(), - cubeOther->_size.y(),   cubeOther->_size.z());
        QVector3D b5(  cubeOther->_size.x(),   cubeOther->_size.y(), - cubeOther->_size.z());
        QVector3D b6(- cubeOther->_size.x(),   cubeOther->_size.y(), - cubeOther->_size.z());
        QVector3D b7(  cubeOther->_size.x(), - cubeOther->_size.y(), - cubeOther->_size.z());
        QVector3D b8(- cubeOther->_size.x(), - cubeOther->_size.y(), - cubeOther->_size.z());

        b1 = bTransform * b1;
        b2 = bTransform * b2;
        b3 = bTransform * b3;
        b4 = bTransform * b4;
        b5 = bTransform * b5;
        b6 = bTransform * b6;
        b7 = bTransform * b7;
        b8 = bTransform * b8;

        std::vector<QVector3D*> bPath(8);
        bPath[0] = &b1;
        bPath[1] = &b2;
        bPath[2] = &b4;
        bPath[3] = &b8;
        bPath[4] = &b7;
        bPath[5] = &b5;
        bPath[6] = &b1;
        bPath[7] = &b2;

        std::vector<QVector3D*> bList(8);
        bList[0] = &b1;
        bList[1] = &b2;
        bList[2] = &b3;
        bList[3] = &b4;
        bList[4] = &b5;
        bList[5] = &b6;
        bList[6] = &b7;
        bList[7] = &b8;

        //If all points of the other cube are not on the same side of c, no collision
        //If not, the separation plane & overlap distance will be processed

        //NOTE: might need to store the opposite of the normal if it points the wrong direction, the idea is for it to point from A to B

        QVector3D separationNormal;
        float overlapDistance = MAXFLOAT;

        for (int i = 2; i < 7; i++) {
            QVector3D& v0 = *aPath[i - 1];
            QVector3D& v1 = *aPath[i - 2];
            QVector3D& v2 = *aPath[i    ];
            QVector3D n = QVector3D::crossProduct((v1 - v0), (v2 - v0)).normalized();

            QVector3D& vOpposite = *aPath[i + 1];

            float a, b, c, d;
            a = n.x();
            b = n.y();
            c = n.z();
            d = QVector3D::dotProduct(n, v0);

            float opposite = a*vOpposite.x() + b*vOpposite.y() + c*vOpposite.z() - d;
            for (int j = 0; j < 8; j++) {
                float current = a*bList[j]->x() + b*bList[j]->y() + c*bList[j]->z() - d;
                if (opposite * current > 0) {
                    float planeDistance = bList[j]->distanceToPlane(v0, n);
                    if (planeDistance < overlapDistance) {
                        overlapDistance = planeDistance;
                        separationNormal = n;
                    }
                }
            }
        }

        for (int i = 2; i < 7; i++) {
            QVector3D& v0 = *bPath[i - 1];
            QVector3D& v1 = *bPath[i - 2];
            QVector3D& v2 = *bPath[i    ];
            QVector3D n = QVector3D::crossProduct((v1 - v0), (v2 - v0)).normalized();

            QVector3D& vOpposite = *bPath[i + 1];

            float a, b, c, d;
            a = n.x();
            b = n.y();
            c = n.z();
            d = QVector3D::dotProduct(n, v0);

            float opposite = a*vOpposite.x() + b*vOpposite.y() + c*vOpposite.z() - d;
            for (int j = 0; j < 8; j++) {
                float current = a*aList[j]->x() + b*aList[j]->y() + c*aList[j]->z() - d;
                if (opposite * current > 0) {
                    float planeDistance = bList[j]->distanceToPlane(v0, n);
                    if (planeDistance < overlapDistance) {
                        overlapDistance = planeDistance;
                        separationNormal = n;
                    }
                }
            }
        }

        //If a collision occured, store the separation direction & distance, then process the collision normal & position
        if (overlapDistance < MAXFLOAT) {
            qDebug("%s", (std::to_string(overlapDistance) + " - " + std::to_string(separationNormal.x()) + ", " + std::to_string(separationNormal.y()) + ", " + std::to_string(separationNormal.z())).c_str());
            return true;

            // Here we have to check the vertex that is the closer to the other cube.
            // The collision will be either on :
            // The closest vertex
            // one of the 3 closest edges (incident to the closest vertex)
            // one of the 3 closest faces (              //              )
            // Not sure about that but i have the intuition that it should work for cubes.

            // To Test (from A to B) :
            //  - Vertex -> Face
            //  - Edge -> Edge
            //  - Face -> Vertex
            // Too rare to occur, it will be detected as an Edge -> Edge or Vertex -> Face and thats okay :
            //  - Vertex -> Vertex
            //  - Vertex -> Edge
            //  - Edge -> Vertex
            //  - Face -> Face

        }
    }

    return false;
}


