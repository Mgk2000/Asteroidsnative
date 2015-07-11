#include "background.h"
#include "texture.h"
#include "mat4.h"
#include "view.h"
#include "logmsg.h"
Background::Background(View* _view, Texture * _texture) : FlyingObject(_view, 2, _texture)
{
}

void Background::init()
{
    points[0] = Point4D(0.0, 0.0, 0.0, 0.0);
    points[1] = Point4D(0.5, 0.0, 1.0, 0.0);
    points[2] = Point4D(0.0, 0.5, 0.0, 1.0);
    points[3] = points[2];
    points[4] = points[1];
    points[5] = Point4D(0.5, 0.5, 1.0, 1.0);
    indices = new GLushort [6];
    for (int i = 0; i< 6; i++)
        indices[i] = i;
    nindices = 6;
    nvertices = 6;
    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(Point4D), points, GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nindices * sizeof(GLushort), indices, GL_STATIC_DRAW);
}
void Background::draw()
{
//	drawTexture();
//	return;
    Mat4 _matrix1 = view->projection1;
    int err;
    glUseProgram(_texture->program());
    err = glGetError();
    if (err)
        LOG0("err=%d", err);
    glActiveTexture(GL_TEXTURE0);
    err = glGetError();
    if (err)
        LOG0("err=%d", err);
    glBindTexture(GL_TEXTURE_2D, _texture->textureId());
    err = glGetError();
    if (err)
        LOG0("err=%d", err);
    glUniformMatrix4fv(_texture->matrixLocation(), 1, false, (const GLfloat*) &_matrix1);
    err = glGetError();
    if (err)
        LOG0("err=%d", err);
    glUniform4fv(_texture->colorMultLocation(), 1,
                       (const GLfloat*) &_colorMult);
    err = glGetError();
    if (err)
        LOG0("err=%d", err);
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    err = glGetError();
    if (err)
        LOG0("err=%d", err);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    err = glGetError();
    if (err)
        LOG0("err=%d", err);
    int texturelocation = _texture->textureLocation();
    glUniform1i(texturelocation, 0);
    err = glGetError();
    if (err)
        LOG0("err=%d", err);

//    unsigned int * offset = 0;
    void * offset = 0;

    int vertexLocation = _texture->posLocation();
    glEnableVertexAttribArray(vertexLocation);
    err = glGetError();
    if (err)
        LOG0("err=%d", err);

    glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Point4D), (const void *)offset);
    err = glGetError();
    if (err)
        LOG0("err=%d", err);
    offset += 8;

    glEnableVertexAttribArray(_texture->texCoordLocation());
    err = glGetError();
    if (err)
        LOG0("err=%d", err);
    glVertexAttribPointer(_texture->texCoordLocation(), 2, GL_FLOAT,
                          GL_FALSE, sizeof(Point4D), (const void *)offset);
    err = glGetError();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    err = glGetError();
    if (err)
        LOG0("err=%d", err);
    err = glGetError();
    glDisableVertexAttribArray(_texture->texCoordLocation());
    glDisableVertexAttribArray(vertexLocation);

}

