#include "flyingobject.h"
#include "view.h"
#include "math.h"
#include "math_helper.h"
#include "logmsg.h"
#include "texture.h"

FlyingObject::FlyingObject(View* _view, int _nbos, Texture* __texture): nvbos (_nbos), vertices(0), indices(0), nvertices(0), nindices(0), view(_view),
    _rotateSpeed(0.0f), angle(0.f), rotateAngle (0.0f),  speed (0.f),  _scale(1.0), _texture(__texture),
    _colorMult(1.0,1.0,1.0), _shootCount(0), _breakCount (1), _scaleX(1.0), _scaleY(1.0),
    _howDrawTriangles (GL_TRIANGLES)
{
	if (nvbos)
	{
		vboIds = new uint[nvbos];
		glGenBuffers(nvbos, vboIds);
	}
    _startTime = currTime();

}

FlyingObject::FlyingObject(View *_view, int _nbos, float _x, float _y, float _speed, float _angle):
	nvbos (_nbos), vertices(0), indices(0), nvertices(0), nindices(0), view(_view),
    _rotateSpeed(0.0f), angle(_angle), rotateAngle (0.0f),
    speed (_speed), x(_x), y(_y) , _scale(1.0), _texture(0),
    _colorMult(1.0,1.0,1.0),
   _shootCount(0), _breakCount (1) , _scaleX(1.0), _scaleY(1.0),
   _howDrawTriangles (GL_TRIANGLES)

{
	if (nvbos)
	{
		vboIds = new uint[nvbos];
		glGenBuffers(nvbos, vboIds);
	}

    _startTime = currTime();
}

FlyingObject::FlyingObject(View *_view, int _nbos, float _x, float _y, float _speed,
    float _angle, Texture *__texture):
nvbos (_nbos), vertices(0), indices(0), nvertices(0), nindices(0), view(_view),
_rotateSpeed(0.0f), angle(_angle), rotateAngle (0.0f), speed (_speed), x(_x), y(_y) ,
  _scale(1.0), _texture(__texture), _colorMult(1.0,1.0,1.0)
, _shootCount(0), _breakCount (1) , _scaleX(1.0), _scaleY(1.0),
  _howDrawTriangles (GL_TRIANGLES)

{
    if (nvbos)
    {
        vboIds = new uint[nvbos];
        glGenBuffers(nvbos, vboIds);
    }
    _startTime = currTime();
}

FlyingObject::~FlyingObject()
{
	if (nvbos)
	{
		glDeleteBuffers(nvbos, vboIds);
		delete[] vboIds;
	}
	if (vertices)
		delete[] vertices;
	if (indices)
		delete[] indices;
}

void FlyingObject::init()
{
	vx = speed* sin(angle);
	vy = speed* cos(angle);
}

void FlyingObject::draw()
{
}

void FlyingObject::moveStep(float delta)
{
    x = x + vx * delta;
    y = y + vy * delta;
}

void FlyingObject::getCurrentCoords(Point *_vertices, int *_nvertices) const
{
	*_nvertices = nvertices;
	for (int i=0; i< nvertices; i++)
	{
		_vertices[i].x = vertices[i].x+x;
		_vertices[i].y = vertices[i].y+y;
	}
}

void FlyingObject::fill_vbos()
{
	// Transfer vertex data to VBO 0
	glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
	glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(Point), vertices, GL_STATIC_DRAW);

	// Transfer index data to VBO 1
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nindices * sizeof(GLushort), indices, GL_STATIC_DRAW);
}

Random& FlyingObject::random1() const
{
	return view->random1();
}

Random &FlyingObject::random2() const
{
	return view->random2();
}

void FlyingObject::drawTriangles (uint vbo)
{
	Mat4 _matrix1;
	_matrix1.translate(x, y, 0);
	_matrix1 = view->projection1 * _matrix1;
	glUseProgram(view->program());
    _matrix1.scale(_scaleX, _scaleY , 1.0);
    glUniformMatrix4fv(view->matrixlocation(), 1, false, (const GLfloat*) &_matrix1);
	glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(view->vertexlocation());
	glVertexAttribPointer(view->vertexlocation(), 3, GL_FLOAT, GL_FALSE, sizeof(Point), (const void *) 0);
	Point4D col = color();
	glUniform4fv(view->colorlocation() ,1 , (GLfloat*) &col );
    glDrawElements(_howDrawTriangles, nindices , GL_UNSIGNED_SHORT, 0);
	glDisableVertexAttribArray(view->vertexlocation());
}

void FlyingObject::drawLines(int how, uint vbo, int npoints, const Point4D& _color, float _width, float angle)
{
//	int err;
	Mat4 _matrix1;
	_matrix1.translate(x, y, 0);
	_matrix1 = view->projection1 * _matrix1;
	//	glUseProgram(view->flyingprogram().programId());
		glUseProgram(view->program());
    float fi = (angle + rotateAngle);
    if (fi !=0.0f)
        _matrix1.rotateZ(fi);
    if (_scale<0.99 ||_scale>1.1)
    {
        Mat4 scaleMatrix(_scale);
        _matrix1=_matrix1 * scaleMatrix;
    }
    _matrix1.scale(_scaleX, _scaleY , 1.0);
    glUniformMatrix4fv(view->matrixlocation(), 1, false, (const GLfloat*) &_matrix1);
//	err = glGetError();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	err = glGetError();
	glEnableVertexAttribArray(view->vertexlocation());
//	err = glGetError();
	glVertexAttribPointer(view->vertexlocation(), 3, GL_FLOAT, GL_FALSE, sizeof(Point), (const void *) 0);
//	err = glGetError();
	Point4D col = _color;
	glUniform4fv(view->colorlocation() ,1 , (GLfloat*) &col );
//	err = glGetError();
    glLineWidth(_width);
//	err = glGetError();
	glDrawArrays(how, 0, npoints);
//	err = glGetError();
//	LOGD("err=%d", err);
	glDisableVertexAttribArray(view->vertexlocation());
//	err = glGetError();
}

bool FlyingObject::isIntersects(const FlyingObject& obj) const
{
	Point mycenter (X(), Y());
	Point ocenter (obj.X(), obj.Y());
    float dx = X() - obj.X();
    float dy = Y() - obj.Y();
    float r2 = (_r + obj.r());
    if (dx * dx + dy * dy > r2 * r2)
        return false;
	int onvertices;
    Point overtices[MAXVERTICES];
    Point myvertices [MAXVERTICES];
	int mynvertices;
	obj.getCurrentCoords(overtices, &onvertices);
	getCurrentCoords(myvertices, &mynvertices);
	for (int i =0; i< mynvertices; i++)
		if (::isInside(&myvertices[i],overtices,&ocenter,onvertices, false))
		{
//			::isInside(&myvertices[i],overtices,&ocenter,onvertices, false);
			return true;
		}
	for (int i =0; i< onvertices; i++)
		if (::isInside(&overtices[i],myvertices,&mycenter,mynvertices, false))
			return true;

	return false;

}

bool FlyingObject::isPointInside(Point *p) const
{
    float dx = p->x - x;
    float dy = p->y -y;
    if (sqr(dx) + sqr(dy) > sqr(this->_r))
    	return false;

	Point myvertices [MAXVERTICES];
	int mynvertices;
	getCurrentCoords(myvertices, &mynvertices);
	Point mycenter (X(), Y());
	return ::isInside(p, myvertices, &mycenter,mynvertices, false);
}

long long FlyingObject::currTime() const
{
    return view->currTime();
}

BitmapText *FlyingObject::bitmapText() const
{
    return view->getBitmapText();
}

float FlyingObject::getShootAngle(const FlyingObject *dst, float shootSpeed) const
{
    float dstX = dst->X();
    float dstY = dst->Y();
    float srcX, srcY;
    float r2 = sqr(dst->r()) / 2;
    float fi;
    for (int i =0; i<3; i++)
    {
        float dist0 = dist(dstX, dstY, x, y);
        float t = dist0 / shootSpeed;
        dstX = dst->X() + t * dst->VX();
        dstY = dst->Y() + t * dst->VY();
        if (dstX < view->left() || dstX > view->right() || dstY >1 || dstY < -1)
            return 1000;
        fi = atan2(dstX - x, dstY - y);
        srcX = x + t * shootSpeed * sin(fi);
        srcY = y * t * shootSpeed * cos(fi);
       // if (dist2 (dstX, dstY, srcX, srcY) < r2)
       //     break;
    }
    return fi;

}

void FlyingObject::showVertices()
{
    for (int i = 0; i< nvertices; i++)
        LOGD("vertices[%d] x=%f y=%f", i, vertices[i].x, vertices[i].y );
}
void FlyingObject::drawTexture(float angle)
{
    Mat4 _matrix1;
    _matrix1.translate(x, y, 0);
    _matrix1 = view->projection1 * _matrix1;
    float fi = (angle + rotateAngle);
    if (fi !=0.0f)
        _matrix1.rotateZ(fi);
    if (_scale<0.99 ||_scale>1.1)
    {
        Mat4 scaleMatrix(_scale);
        _matrix1=_matrix1 * scaleMatrix;
    }
    int err;
    glUseProgram(_texture->program());
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    glActiveTexture(GL_TEXTURE0);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    glBindTexture(GL_TEXTURE_2D, _texture->textureId());
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glBlendFunc (GL_SRC_COLOR, GL_DST_COLOR);
//    glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glUniformMatrix4fv(_texture->matrixLocation(), 1, false,
                       (const GLfloat*) &_matrix1);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    glUniform4fv(_texture->colorMultLocation(), 1,
                       (const GLfloat*) &_colorMult);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    int texturelocation = _texture->textureLocation();
    glUniform1i(texturelocation, 0);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);

//    quintptr offset = 0;
    void* offset = 0;
    int vertexLocation = _texture->posLocation();
    glEnableVertexAttribArray(vertexLocation);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);

    glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Point4D), (const void *)offset);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    offset += 8;

    glEnableVertexAttribArray(_texture->texCoordLocation());
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    glVertexAttribPointer(_texture->texCoordLocation(), 2, GL_FLOAT,
                          GL_FALSE, sizeof(Point4D), (const void *)offset);
    err = glGetError();
//    glDrawElements(how, nindices, GL_UNSIGNED_SHORT, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, nvertices+2);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    err = glGetError();
    glDisableVertexAttribArray(_texture->texCoordLocation());
    glDisableVertexAttribArray(vertexLocation);

}
