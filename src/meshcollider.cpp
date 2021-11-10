#include "meshcollider.h"

#include <fstream>
#include <string>

//TY Stack overflow
std::vector<std::string> MeshCollider::split (std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr(pos_start));
    return res;
}

MeshCollider::MeshCollider(char* path) {
    _vertices = new std::vector<QVector3D*>();
    _indices = new std::vector<int>();

    std::ifstream file(path);
    std::string str;
    while (std::getline(file, str)) {
        std::vector<std::string> splitStr = split(str, " ");

        if (strcmp(splitStr[0].c_str(), "v") == 0) { //VERTEX
            float x, y, z;

            x = atof(splitStr[1].c_str());
            y = atof(splitStr[2].c_str());
            z = atof(splitStr[3].c_str());

            _vertices->push_back(new QVector3D(x, y, z));
        }
        if (strcmp(splitStr[0].c_str(), "f") == 0) { //FACE
            for (int i = 1; i <= 3; i++) {
                std::vector<std::string> vertexStr = split(splitStr[i], "/");

                int posIndex = atoi(vertexStr[0].c_str()) - 1;

                _indices->push_back(posIndex);
            }
        }
    }

    MeshCollider::SetAABB();
}

void MeshCollider::SetAABB() {
    QVector3D min( 100000000,  100000000,  100000000);
    QVector3D max(-100000000, -100000000, -100000000);

    for(QVector3D* vertex : *_vertices) {
        min.setX(std::min(min.x(), vertex->x()));
        min.setY(std::min(min.y(), vertex->y()));
        min.setZ(std::min(min.z(), vertex->z()));
        max.setX(std::max(max.x(), vertex->x()));
        max.setY(std::max(max.y(), vertex->y()));
        max.setZ(std::max(max.z(), vertex->z()));
    }

    _localAABB = AABB(min, max);
}
