#ifndef TEXTURE_H
#define TEXTURE_H
#include <globject.h>
class Texture : public GlObject
{
public:
    Texture(FILE* file);
private:
    GLuint uiTexture;
    int gvPositionHandle,
    //gvNormalHandle,
    gvSamplerHandle,
    gvTexCoordHandle;
    int program;

    GLubyte* loadPicture(FILE* _pFile);
    GLuint createSimpleTexture2D(GLuint _textureid, GLubyte *pixels, int width, int height, int channels);
    void createProgram(const char *vtxSrc, const char *fragSrc);
    GLuint createShader(GLenum shaderType, const char* src);

    bool setupGraphics(int w, int h, GLubyte* data);
};

#endif // TEXTURE_H
