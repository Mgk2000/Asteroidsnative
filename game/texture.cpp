#include "texture.h"
#include "logmsg.h"
static const char gVertexShader[] = "attribute vec4 a_position;   \n"
        "//attribute vec3 a_normal;     \n"
        "attribute vec2 a_texCoord;   \n"
        "varying vec2 v_texCoord;     \n"
        "//varying vec3 v_normal; \n"
        "void main()                  \n"
        "{                            \n"
        "   gl_Position = a_position; \n"
        " //v_normal = a_normal; \n"
        "   v_texCoord = a_texCoord;  \n"
        "}                            \n";

static const char gFragmentShader[] =
        "precision mediump float;                            \n"
        "varying vec2 v_texCoord;                            \n"
        "//varying vec3 v_normal; \n"
        "uniform sampler2D s_texture;                        \n"
        "void main()                                         \n"
        "{                                                   \n"
        "  gl_FragColor = texture2D( s_texture, v_texCoord );\n"
        "}                                                   \n";
Texture::Texture(FILE* _pFile) : program(0)
{
    GLubyte* data = loadPicture(_pFile);
    setupGraphics(128, 128, data);

}
bool Texture::setupGraphics(int w, int h, GLubyte* data)
{
//    printGLString("Version", GL_VERSION);
//    printGLString("Vendor", GL_VENDOR);
//    printGLString("Renderer", GL_RENDERER);
//    printGLString("Extensions", GL_EXTENSIONS);

   // LOGD("setupGraphics(%d, %d)", w, h);
    createProgram(gVertexShader, gFragmentShader);
    if (!program)
    {
        LOGD("Could not create program.");
        return false;
    }
    gvPositionHandle = glGetAttribLocation(program, "a_position");
    //gvNormalHandle=glGetAttribLocation(program,"a_normal");
    gvTexCoordHandle = glGetAttribLocation(program, "a_texCoord");

    gvSamplerHandle = glGetUniformLocation(program, "s_texture");

    // Use tightly packed data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Generate a texture object
    glGenTextures(1, &uiTexture);
    bool bAlpha = false;
    if(!bAlpha)
        uiTexture = createSimpleTexture2D(uiTexture, data,w, h, 3);
    else
        uiTexture = createSimpleTexture2D(uiTexture, data,w, h, 4);

    //checkGlError("glGetAttribLocation");
    //LOGD("glGetAttribLocation(\"vPosition\") = %d\n", gvPositionHandle);

    glViewport(0, 0, w, h);
    //checkGlError("glViewport");
    return true;
}
GLubyte* Texture::loadPicture(FILE* _pFile)
{
    // Read data from file into texture
    int width = 512;
    int height = 512;
    unsigned char* pcData = (unsigned char*)malloc(width * height * 3);
    fread(pcData, width * height * 3, 1, _pFile);
    free(pcData);
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
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
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

GLuint Texture::createSimpleTexture2D(GLuint _textureid, GLubyte* pixels,
                int width, int height, int channels) {

        // Bind the texture
        glActiveTexture(GL_TEXTURE0);
        //checkGlError("glActiveTexture");
        // Bind the texture object
        glBindTexture(GL_TEXTURE_2D, _textureid);
        //checkGlError("glBindTexture");

        GLenum format;
        switch (channels) {
        case 3:
                format = GL_RGB;
                break;
        case 1:
                format = GL_LUMINANCE;
                break;
        case 4:
                format = GL_RGBA;
                break;
        }
        // Load the texture
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                        GL_UNSIGNED_BYTE, pixels);

        //checkGlError("glTexImage2D");
        // Set the filtering mode
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

        return _textureid;

}
