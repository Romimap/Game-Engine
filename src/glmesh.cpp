#include "glmesh.h"

///Vertex Data : position, normal & UV
struct VertexData {
    QVector3D position;
    QVector3D normal;
    QVector2D uv;
};

///Reads <lod0> as a mesh.
GLMesh::GLMesh(std::string lod0)
    : indexBuf(QOpenGLBuffer::IndexBuffer){
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initializes cube geometry and transfers it to VBOs
    initMesh(lod0, &arrayBuf, &indexBuf);
}

//TY Stack overflow
std::vector<std::string> split (std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

/*
                ______    ____      _________________________________________________
               :  ;;;;\__/:  ;\    |                                                 |
                \;__/.... :  _/;   | C LE CODE DE ROMIMAP                            |
               ___:__ ..__\_/__;   | OUBLIE PAS DE LE CREDITER POUR SES POINTS BONUS |
               |  ## `--'   ##|;   |                                                 |
               |_____/~;\_____|;  < _________________________________________________|
                 /~~~_ _ ~~   /
                 // (_:_)   \
           _     // ,'~ `,_\~\_
          //     ~~`,---,'~~~   \
 ___     //         ~~~~      ;; \_  __
/_\/____::_        ,(:;:  __    ;; \/;;\  __
\_/) _  :: (       ; ;;:    \    / ;:;;::-,-'
   |[-]_::-|       : :;;:   /  * | ;:;;:;'
   |       |       :.`,:::  : +  : /___:'
   |[_ ] [\|       ;. ;--`:_:.  *| /   /
   |__| |_]|    ,-' . :uu-'/     \|UUU/
   |_______|   ;_|_|_/    :_;_;_;_:
    [=====]
*/
///Parses a .obj into an arrayBuffer & indexBuffer
void GLMesh::initMesh(std::string path, QOpenGLBuffer* arrayBuffer, QOpenGLBuffer* indexBuffer) {
    std::vector<QVector3D> pos;
    std::vector<QVector3D> nrm;
    std::vector<QVector2D> uv;
    std::vector<VertexData>* vertexArray_ptr = new std::vector<VertexData>();
    std::vector<GLushort>* indexArray_ptr = new std::vector<GLushort>();
    std::vector<VertexData>& vertexArray = *vertexArray_ptr;
    std::vector<GLushort>& indexArray = *indexArray_ptr;

    std::ifstream file(path.c_str());
    std::string str;
    while (std::getline(file, str)) {
        std::vector<std::string> splitStr = split(str, " ");

        if (strcmp(splitStr[0].c_str(), "v") == 0) { //VERTEX
            float x, y, z;

            x = atof(splitStr[1].c_str());
            y = atof(splitStr[2].c_str());
            z = atof(splitStr[3].c_str());

            pos.push_back(QVector3D(x, y, z));
        }
        if (strcmp(splitStr[0].c_str(), "vn") == 0) { //NORMAL
            float x, y, z;

            x = atof(splitStr[1].c_str());
            y = atof(splitStr[2].c_str());
            z = atof(splitStr[3].c_str());

            nrm.push_back(QVector3D(x, y, z));
        }
        if (strcmp(splitStr[0].c_str(), "vt") == 0) { //TEX COORD
            float x, y;

            x = atof(splitStr[1].c_str());
            y = atof(splitStr[2].c_str());

            uv.push_back(QVector2D(x, y));
        }
        if (strcmp(splitStr[0].c_str(), "f") == 0) { //FACE
            for (int i = 1; i <= 3; i++) {
                std::vector<std::string> vertexStr = split(splitStr[i], "/");

                int posIndex = atoi(vertexStr[0].c_str()) - 1;
                int texIndex = atoi(vertexStr[1].c_str()) - 1;
                int nrmIndex = atoi(vertexStr[2].c_str()) - 1;

                VertexData vertexData;
                vertexData.position = pos[posIndex];
                vertexData.normal = nrm[nrmIndex];
                vertexData.uv = uv[texIndex];

                //We check if the vertex data already exists in our vertex array
                bool contains = false;
                for (int j = 0; j < (int)vertexArray.size(); j++) {
                    VertexData& vd = vertexArray[j];
                    //oof
                    if (vd.position.x() == vertexData.position.x()
                    &&  vd.position.y() == vertexData.position.y()
                    &&  vd.position.z() == vertexData.position.z()
                    &&  vd.normal.x() == vertexData.normal.x()
                    &&  vd.normal.y() == vertexData.normal.y()
                    &&  vd.normal.z() == vertexData.normal.z()
                    &&  vd.uv.x() == vertexData.uv.x()
                    &&  vd.uv.y() == vertexData.uv.y()) {
                        contains = true;
                        indexArray.push_back(j);
                        break;
                    }
                }
                if (!contains) {
                    indexArray.push_back(vertexArray.size());
                    vertexArray.push_back(vertexData);
                }
            }
        }
    }

    // Transfer vertex data to VBO 0
    arrayBuffer->bind();
    arrayBuffer->allocate(&vertexArray.front(), vertexArray.size() * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuffer->bind();
    indexBuffer->allocate(&indexArray.front(), indexArray.size() * sizeof(GLushort));
}

///Draws that mesh. This should be called from a RendererComponent, using the shader saved into a Material.
void GLMesh::draw(QOpenGLShaderProgram *program) {
    // Tell OpenGL which VBOs to use
    int size = 0;
    arrayBuf.bind();
    indexBuf.bind();
    size = indexBuf.size();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for vertex normals
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int normalLocation = program->attributeLocation("a_normal");
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, 0); //Careful update indicesNumber when creating new geometry


}
