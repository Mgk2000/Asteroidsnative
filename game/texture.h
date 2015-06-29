#ifndef TEXTURE_H
#define TEXTURE_H
#include <globject.h>
class Texture : public GlObject
{
public:
    Texture(FILE* file);
    int program;
    int posLocation() const {return _posLocation;}
    int textureLocation() const {return _textureLocation;}
    int texCoordLocation() const {return _texCoordLocation;}
    int matrixLocation() const {return _matrixLocation;}

private:
    GLuint uiTexture;
    GLubyte* getFragment( GLubyte *src, GLubyte* dst, int x, int y, int width, int height) const;
    GLubyte* loadPicture(FILE* _pFile);
    GLuint createTexture(GLuint _textureid, GLubyte *pixels, int width, int heights);
    void createProgram(const char *vtxSrc, const char *fragSrc);
    GLuint createShader(GLenum shaderType, const char* src);
    GLubyte* pcData;
    bool setupGraphics(int w, int h, GLubyte* data);
    int _posLocation,
    _textureLocation,
    _texCoordLocation,
    _matrixLocation;
    int _picWidth, _picHeight;
};

#endif // TEXTURE_H
