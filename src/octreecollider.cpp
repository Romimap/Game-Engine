#include "octreecollider.h"
#define SCALEFACTOR 4

OctreeCollider::OctreeCollider() {

}

void OctreeCollider::Init() {
    _octreeComponent = _parent->GetComponent<OctreeComponent>();
    SetAABB();
}

void OctreeCollider::SetAABB() {
    _localAABB._min = QVector3D(0, 0, 0);
    _localAABB._max = QVector3D(_octreeComponent->getXSize(), _octreeComponent->getYSize(), _octreeComponent->getZSize());
}

RayCastHit OctreeCollider::RayCast(QVector3D origin, QVector3D destination) {
    return GridTreeIntersect(origin, destination, GetParent()->GetTransform()->GetGlobalPosition(), QVector3D(), QVector3D(4, 16, 4), 2);
}


bool OctreeCollider::BoxIntersect(QVector3D origin, QVector3D direction, QVector3D min, QVector3D max, float &tmin, float &tmax) {
    tmax = (max.x() - origin.x()) / direction.x();
    tmin = (min.x() - origin.x()) / direction.x();

    if (tmin > tmax) {
        float tmp = tmin;
        tmin = tmax;
        tmax = tmp;
    }

    float tymin = (min.y() - origin.y()) / direction.y();
    float tymax = (max.y() - origin.y()) / direction.y();

    if (tymin > tymax) {
        float tmp = tymin;
        tymin = tymax;
        tymax = tmp;
    }

    if ((tmin > tymax) || (tymin > tmax)) {
        return false;
    }

    if (tymin > tmin) {
        tmin = tymin;
    }

    if (tymax < tmax) {
        tmax = tymax;
    }

    float tzmin = (min.z() - origin.z()) / direction.z();
    float tzmax = (max.z() - origin.z()) / direction.z();

    if (tzmin > tzmax) {
        float tmp = tzmin;
        tzmin = tzmax;
        tzmax = tmp;
    }

    if ((tmin > tzmax) || (tzmin > tmax)) {
        return false;
    }

    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    if (tmin > 0 && tmin < 1000) return true;
    if (tmax > 0 && tmax < 1000) return true;
    return false;
}


RayCastHit OctreeCollider::GridTreeIntersect (QVector3D O, QVector3D D, QVector3D gridPos, QVector3D offset, QVector3D gridSize, int layer) {
    float voxelSize = 64. / gridSize.x();

    RayCastHit hit;
    hit._distance = std::numeric_limits<float>::max();

    WorldGeneratorComponent *WGC = _parent->GetParent()->GetComponent<WorldGeneratorComponent>();

    if (layer <= -1) {
        offset /= SCALEFACTOR;
        if (WGC->getVoxelType(offset.x(), offset.y(), offset.z(), layer + 1) != MaterialId::AIR) {
            hit._distance = 0;
        }
        return hit;
    }


    float tmin, tmax;
    BoxIntersect(O, D, gridPos, gridPos + gridSize * voxelSize, tmin, tmax);
    if (tmax == std::numeric_limits<float>::max()) return hit;

    QVector3D hitPos = O;
    if (tmin > 0) {
        hitPos += D * (tmin + EPSILON);
    }

    float X, Y, Z;

    X = floor(((hitPos - gridPos) / voxelSize).x());
    Y = floor(((hitPos - gridPos) / voxelSize).y());
    Z = floor(((hitPos - gridPos) / voxelSize).z());

    bool isFull = WGC->getVoxelType(X + offset.x(), Y + offset.y(), Z + offset.z(), layer) != MaterialId::AIR;
    if (isFull) {
        hit._distance = 0;

        RayCastHit rhit = GridTreeIntersect(O + D * (hit._distance - EPSILON), D, gridPos + QVector3D(X * voxelSize, Y * voxelSize, Z * voxelSize), offset * SCALEFACTOR + QVector3D(X, Y, Z) * SCALEFACTOR, gridSize * SCALEFACTOR, layer - 1);
        if (rhit._distance < std::numeric_limits<float>::max()) return rhit;
    }

    float tx, ty, tz;

    tx = (D / D.x()).length();
    ty = (D / D.y()).length();
    tz = (D / D.z()).length();

    float tmaxx, tmaxy, tmaxz;

    tmaxx = ((hitPos - gridPos) / voxelSize).x() - X;
    if (D.x() > 0) tmaxx = 1 - tmaxx;
    tmaxy = ((hitPos - gridPos) / voxelSize).y() - Y;
    if (D.y() > 0) tmaxy = 1 - tmaxy;
    tmaxz = ((hitPos - gridPos) / voxelSize).z() - Z;
    if (D.z() > 0) tmaxz = 1 - tmaxz;
    tmaxx = tmaxx * tx;
    tmaxy = tmaxy * ty;
    tmaxz = tmaxz * tz;

    float Xstep, Ystep, Zstep;

    if (D.x() > 0) Xstep = 1;
    else Xstep = -1;
    if (D.y() > 0) Ystep = 1;
    else Ystep = -1;
    if (D.z() > 0) Zstep = 1;
    else Zstep = -1;

    while (true) {
        if (tmaxx < tmaxy && tmaxx < tmaxz) {
            X += Xstep;
            if (X >= 0 && X < gridSize.x()) {
                isFull = WGC->getVoxelType(X + offset.x(), Y + offset.y(), Z + offset.z(), layer) != MaterialId::AIR;
                if (isFull) {
                    hit._distance = tmaxx * voxelSize;

                    RayCastHit rhit = GridTreeIntersect(O + D * (hit._distance - EPSILON), D, gridPos + QVector3D(X, Y, Z) * voxelSize, offset * SCALEFACTOR + QVector3D(X, Y, Z) * SCALEFACTOR, gridSize * SCALEFACTOR, layer - 1);
                    if (rhit._distance < std::numeric_limits<float>::max()) {
                        rhit._distance += hit._distance;
                        return rhit;
                    }
                }
            } else {
                break;
            }
            tmaxx += tx;
        } else if (tmaxy < tmaxz) {
            Y += Ystep;
            if (Y >= 0 && Y < gridSize.y()) {
                isFull = WGC->getVoxelType(X + offset.x(), Y + offset.y(), Z + offset.z(), layer) != MaterialId::AIR;
                if (isFull) {
                    hit._distance = tmaxy * voxelSize;

                    RayCastHit rhit = GridTreeIntersect(O + D * (hit._distance - EPSILON), D, gridPos + QVector3D(X, Y, Z) * voxelSize, offset * SCALEFACTOR + QVector3D(X, Y, Z) * SCALEFACTOR, gridSize * SCALEFACTOR, layer - 1);
                    if (rhit._distance < std::numeric_limits<float>::max()) {
                        rhit._distance += hit._distance;
                        return rhit;
                    }
                }
            } else {
                break;
            }
            tmaxy += ty;
        } else {
            Z += Zstep;
            if (Z >= 0 && Z < gridSize.z()) {
                isFull = WGC->getVoxelType(X + offset.x(), Y + offset.y(), Z + offset.z(), layer) != MaterialId::AIR;
                if (isFull) {
                    hit._distance = tmaxz * voxelSize;

                    RayCastHit rhit = GridTreeIntersect(O + D * (hit._distance - EPSILON), D, gridPos + QVector3D(X, Y, Z) * voxelSize, offset * SCALEFACTOR + QVector3D(X, Y, Z) * SCALEFACTOR, gridSize * SCALEFACTOR, layer - 1);
                    if (rhit._distance < std::numeric_limits<float>::max()) {
                        rhit._distance += hit._distance;
                        return rhit;
                    }
                }
            } else {
                break;
            }
            tmaxz += tz;
        }
    }

    hit._distance = std::numeric_limits<float>::max();
    return hit;
}
