#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class Material {
public:
    QOpenGLTexture* _TexSlot0 = nullptr;
    QOpenGLTexture* _TexSlot1 = nullptr;
    QOpenGLTexture* _TexSlot2 = nullptr;
    QOpenGLTexture* _TexSlot3 = nullptr;
    QOpenGLTexture* _TexSlot4 = nullptr;
    QOpenGLTexture* _TexSlot5 = nullptr;
    QOpenGLTexture* _TexSlot6 = nullptr;
    QOpenGLTexture* _TexSlot7 = nullptr;

    QOpenGLShaderProgram program;
    Material(char* vshaderPath, char* fshaderPath);

    void SetSlot2D (char* path, int slot) {
        QOpenGLTexture* tex = new QOpenGLTexture(QImage(path));
        switch (slot) {
            case 0:
                if (_TexSlot0 != nullptr) delete _TexSlot0;
                _TexSlot0 = tex;
                break;
            case 1:
                if (_TexSlot1 != nullptr) delete _TexSlot1;
                _TexSlot1 = tex;
                break;
            case 2:
                if (_TexSlot2 != nullptr) delete _TexSlot2;
                _TexSlot2 = tex;
                break;
            case 3:
                if (_TexSlot3 != nullptr) delete _TexSlot3;
                _TexSlot3 = tex;
                break;
            case 4:
                if (_TexSlot4 != nullptr) delete _TexSlot4;
                _TexSlot4 = tex;
                break;
            case 5:
                if (_TexSlot5 != nullptr) delete _TexSlot5;
                _TexSlot5 = tex;
                break;
            case 6:
                if (_TexSlot6 != nullptr) delete _TexSlot6;
                _TexSlot6 = tex;
                break;
            case 7:
                if (_TexSlot7 != nullptr) delete _TexSlot7;
                _TexSlot7 = tex;
                break;
        }
    }
};

#endif // MATERIAL_H
