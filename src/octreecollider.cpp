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

RayCastHit OctreeCollider::RayCast(QVector3D origin, QVector3D direction) {
    std::vector<RayCastHit> hits;
    hits.push_back(GridTreeIntersect(origin, direction, GetParent()->GetTransform()->GetPosition() + QVector3D(0, 0, 0)     , QVector3D(0, 0, 0)    , QVector3D(4, 4, 4), 16, 2));
    hits.push_back(GridTreeIntersect(origin, direction, GetParent()->GetTransform()->GetPosition() + QVector3D(0, 64, 0)    , QVector3D(0, 4, 0)    , QVector3D(4, 4, 4), 16, 2));
    hits.push_back(GridTreeIntersect(origin, direction, GetParent()->GetTransform()->GetPosition() + QVector3D(0, 128, 0)   , QVector3D(0, 8, 0)    , QVector3D(4, 4, 4), 16, 2));
    hits.push_back(GridTreeIntersect(origin, direction, GetParent()->GetTransform()->GetPosition() + QVector3D(0, 192, 0)   , QVector3D(0, 12, 0)   , QVector3D(4, 4, 4), 16, 2));

    float minDistance = std::numeric_limits<float>::max();
    int minIndex = 0;

    for (int i = 0; i < 4; i++) {
        if (hits[i]._distance < minDistance) {
            minDistance = hits[i]._distance;
            minIndex = i;
        }
    }

    return hits[minIndex];
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

    if (tmin > 0) return true;
    if (tmin <= 0 && tmax > 0) return true;
    return false;
}


RayCastHit OctreeCollider::GridTreeIntersect (QVector3D O, QVector3D D, QVector3D gridPos, QVector3D offset, QVector3D gridSize, float voxelSize, int layer) {


    RayCastHit hit;
    hit._distance = std::numeric_limits<float>::max();

    OctreeComponent *octreeComponent = GetParent()->GetComponent<OctreeComponent>();

    if (layer <= -1) {
            hit._distance = 0;
        return hit;
    }


    float tmin, tmax;
    bool intersect = BoxIntersect(O, D, gridPos, gridPos + gridSize * voxelSize, tmin, tmax);
    if (!intersect) return hit;

    QVector3D hitPos = O;
    float distanceOffset = 0;
    if (tmin > 0) {
        distanceOffset = tmin + EPSILON;
        hitPos += D * distanceOffset;
    }

    float X, Y, Z;

    X = floor(((hitPos - gridPos) / voxelSize).x());
    Y = floor(((hitPos - gridPos) / voxelSize).y());
    Z = floor(((hitPos - gridPos) / voxelSize).z());

    bool isFull = octreeComponent->getVoxelType(X + offset.x(), Y + offset.y(), Z + offset.z(), layer) != MaterialId::AIR;
    if (isFull) {
        hit._distance = distanceOffset;

        RayCastHit rhit = GridTreeIntersect(O + D * (hit._distance + EPSILON), D, gridPos + QVector3D(X, Y, Z) * voxelSize, (offset + QVector3D(X, Y, Z)) * SCALEFACTOR, gridSize, voxelSize / SCALEFACTOR, layer - 1);
        if (rhit._distance < std::numeric_limits<float>::max()) {
            rhit._distance += (O - hitPos).length();
            return rhit;
        }
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
                isFull = octreeComponent->getVoxelType(X + offset.x(), Y + offset.y(), Z + offset.z(), layer) != MaterialId::AIR;
                if (isFull) {
                    hit._distance = distanceOffset + tmaxx * voxelSize;

                    RayCastHit rhit = GridTreeIntersect(O + D * (hit._distance - EPSILON), D, gridPos + QVector3D(X, Y, Z) * voxelSize, (offset + QVector3D(X, Y, Z)) * SCALEFACTOR, gridSize, voxelSize / SCALEFACTOR, layer - 1);
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
                isFull = octreeComponent->getVoxelType(X + offset.x(), Y + offset.y(), Z + offset.z(), layer) != MaterialId::AIR;
                if (isFull) {
                    hit._distance = distanceOffset + tmaxy * voxelSize;

                    RayCastHit rhit = GridTreeIntersect(O + D * (hit._distance - EPSILON), D, gridPos + QVector3D(X, Y, Z) * voxelSize, (offset + QVector3D(X, Y, Z)) * SCALEFACTOR, gridSize, voxelSize / SCALEFACTOR, layer - 1);
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
                isFull = octreeComponent->getVoxelType(X + offset.x(), Y + offset.y(), Z + offset.z(), layer) != MaterialId::AIR;
                if (isFull) {
                    hit._distance = distanceOffset + tmaxz * voxelSize;

                    RayCastHit rhit = GridTreeIntersect(O + D * (hit._distance - EPSILON), D, gridPos + QVector3D(X, Y, Z) * voxelSize, (offset + QVector3D(X, Y, Z)) * SCALEFACTOR, gridSize, voxelSize / SCALEFACTOR, layer - 1);
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
