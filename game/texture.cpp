#include "texture.h"
#include "logmsg.h"
static const char gVertexShader[] =
        "precision mediump float;                            \n"
        "precision mediump int;                            \n"
        "//attribute vec4 a_position;   \n"
        "attribute vec2 a_position;   \n"
        "attribute vec2 a_texCoord;   \n"
        "uniform mat4 mvp_matrix;     \n"
        "varying vec2 v_texCoord;     \n"
        "void main()                  \n"
        "{                            \n"
        "//   gl_Position = a_position; \n"
        "	gl_Position = mvp_matrix * vec4(a_position,0.0, 1.0);\n"
        "   v_texCoord = a_texCoord;  \n"
        "}                            \n";

static const char gFragmentShader[] =
        "precision mediump float;                            \n"
        "precision mediump int;                            \n"

        "varying vec2 v_texCoord;                            \n"
        "uniform sampler2D s_texture;                        \n"
        "void main()                                         \n"
        "{                                                   \n"
        "  gl_FragColor = texture2D( s_texture, v_texCoord );\n"
        "}                                                   \n";
Texture::Texture(FILE* _pFile) : program(0)
{
    GLubyte* data = loadPicture(_pFile);
    setupGraphics(512, 512, data);
   // free(data);


}

GLubyte *Texture::getFragment(GLubyte *src, GLubyte* dst,
    int x, int y, int width, int height) const
{
    for (int i=0; i < height; i++)
    {
        GLubyte* sptr = &src[(y+i) * _picWidth *3 + x];
        GLubyte* dptr =&dst[i*width];
        memcpy(dptr, sptr, width);
    }
}
bool Texture::setupGraphics(int w, int h, GLubyte* data)
{
    createProgram(gVertexShader, gFragmentShader);
    if (!program)
    {
        LOGD("Could not create program.");
        return false;
    }
    _posLocation = glGetAttribLocation(program, "a_position");
    _texCoordLocation = glGetAttribLocation(program, "a_texCoord");
    _textureLocation = glGetUniformLocation(program, "s_texture");
    _matrixLocation  = glGetUniformLocation(program, "mvp_matrix");
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &uiTexture);
    uiTexture = createTexture(uiTexture, data,w, h);
    return true;
}


GLubyte* Texture::loadPicture(FILE* _pFile)
{
    // Read data from file into texture
    _picWidth = 512;
    _picHeight = 512;
    pcData = new GLubyte[_picWidth* _picHeight*3];
    //unsigned char* pcData = (unsigned char*)malloc(width * height * 3);
    int offset = 0;
    fseek(_pFile, offset + 14 + 40, SEEK_SET);
    fread(pcData, _picWidth * _picHeight * 3, 1, _pFile);
    for (int i=0; i< _picWidth * _picHeight; i++)
    {
        unsigned char c = pcData[i*3];
        pcData[i*3] = pcData[i*3+2];
        pcData[i*3+2] = c;
    }
    char buf[128];
    memcpy (buf, pcData, 128);
    return pcData;
}
void Texture::createProgram(const char* vtxSrc, const char* fragSrc)
{
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader)
        goto exit;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader)
        goto exit;

    program = glCreateProgram();
    if (!program) {
        goto exit;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {

        glDeleteProgram(program);
        program = 0;
    }

exit:
    ;
   // glDeleteShader(vtxShader);
   // glDeleteShader(fragShader);
}

GLuint Texture::createShader(GLenum shaderType, const char *src)
{
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint Texture::createTexture(GLuint _textureId, GLubyte* pixels,
                int width, int height)
{
        int err;
        // Bind the texture
        glActiveTexture(GL_TEXTURE0);
        //checkGlError("glActiveTexture");
        err = glGetError();
        // Bind the texture object
        glBindTexture(GL_TEXTURE_2D, _textureId);
        err = glGetError();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                        GL_UNSIGNED_BYTE, pixels);

        err = glGetError();
        glGenerateMipmap(GL_TEXTURE_2D);
        err = glGetError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        err = glGetError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        err = glGetError();

        return _textureId;

}
