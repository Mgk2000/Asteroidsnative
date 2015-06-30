#ifndef TEXTURE_H
#define TEXTURE_H
#include <globject.h>
class Texture : public GlObject
{
public:
    Texture(const char* filename, int ind);
    int posLocation() const {return _posLocation;}
    int textureLocation() const {return _textureLocation;}
    int texCoordLocation() const {return _texCoordLocation;}
    int matrixLocation() const {return _matrixLocation;}
    int colorMultLocation() const {return _colorMultLocation;}
    int program() const {return _program;}
    int picWidth() const {return _picWidth;}
    int picHeight() const {return _picHeight;}
    bool initGL();
    GLuint textureId() const {return _textureId;}
private:
    GLubyte* getFragment( GLubyte *src, GLubyte* dst, int x, int y, int width, int height) const;
    void loadPicture(const char* filename);
    void createTexture();
    void createProgram();
    GLuint createShader(GLenum shaderType, const char* src);
    GLubyte* pcData;
    int _posLocation,
    _textureLocation,
    _texCoordLocation,
    _colorMultLocation,
    _matrixLocation;
    int _picWidth, _picHeight;
    int _program;
    GLuint _textureId;
};

#endif // TEXTURE_H
