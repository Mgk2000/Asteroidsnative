#ifndef TEXTURE_H
#define TEXTURE_H
#include <globject.h>
class Texture : public GlObject
{
public:
    Texture(FILE* file);
    int gvPositionHandle,
    gvSamplerHandle,
    gvTexCoordHandle,
    gvMatrixHandle;
    int program;
private:
    GLuint uiTexture;

    GLubyte* loadPicture(FILE* _pFile);
    GLuint createSimpleTexture2D(GLuint _textureid, GLubyte *pixels, int width, int height, int channels);
    void createProgram(const char *vtxSrc, const char *fragSrc);
    GLuint createShader(GLenum shaderType, const char* src);

    bool setupGraphics(int w, int h, GLubyte* data);
};

#endif // TEXTURE_H
