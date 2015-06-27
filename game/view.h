#ifndef VIEW_H
#define VIEW_H
#include <globject.h>
//#include "game.h"
#include <list>
#include "points.h"
#include "random.h"
#include "mat4.h"
#include "text.h"

class Ship;
class Gun;
class Bullet;
class Random;
class Asteroid;
class GeometryEngine;
class Patrol;
class Mutex;
struct BulletInfo
{
	Bullet* bullet;
	BulletInfo* next;
};
enum TouchPointState {
    TouchPointPressed    = 0x01,
    TouchPointMoved      = 0x02,
};
struct TouchEvent
{
    TouchEvent(int _type, int _x, int _y):
        type((TouchPointState) _type), x(_x), y(_y){}
TouchPointState type;
int x, y;
};

class View : public GlObject
{
public:
    explicit View();
	~View();
	Mat4 projection1;
	Random& random1() {return _random1;}
	Random& random2() {return _random2;}
	void checkShoots();
    inline float left() const {return -0.6;}
    inline float right() const {return 0.6;}
    inline float top() const {return 1.0;}
    inline float bottom() const {return -1.0;}
	void patrolShoot (Patrol* _patrol);

	void breakShip();
	int matrixlocation() const {return _matrixlocation;}
	int colorlocation() const {return _colorlocation;}
	int vertexlocation() const {return _vertexlocation;}
	GLuint program() const {return _program;}
    void processTouches();
    void processTouchMove (int x, int y);
    void processTouchPress (int x, int y);
    int drawFrame(long long currtime);
    bool initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    int width, height;
    std::list<TouchEvent>touches;
    void onTouchEvent(int what, int x, int y);
    Ship* getShip() {return ship;}
    void setPause(bool p) {pause = p;}
    bool getPause() {return pause;}
    void newGame();
    int lives() const {return _lives;}
    int scores() const {return _scores;}
    inline bool gameIsOver() const {return _lives <= 0;}
private:

	GLuint createShader(GLenum shaderType, const char* src);
	GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);
    bool initShaders();
	void screenToView(int x, int y, float* fx, float * fy) const;
    void startGame();
    void shoot (float angle);
	std::list <Asteroid*> asteroids;
	void addAsteroid(Asteroid* asteroid);
	void deleteAsteroid(Asteroid* asteroid);
	int asteroidAppearTime;
	std::list <Bullet*> bullets;
	void addBullet(Bullet* bullet);
	void deleteBullet(Bullet* bullet);
	void createSplinters(Asteroid* asteroid);
	Patrol* patrol;
    Ship* ship;
    Mutex* mutex;
    void drawCurrentResult() const;
	GLuint _program;
	Gun * gun;
	float aspect;
	Random _random1, _random2;
	int nticks;
	int dieticks;
	int _matrixlocation, _vertexlocation, _colorlocation;
    long long lastTime;
    long long startTime;
    int period;
    Text *text;
    bool pause;
    int _lives;
    int _scores;
    void sound(int is) {if (is>maxSound) maxSound=is;}
    int maxSound;
    void checkEndGame();
    void moveObjects(float delta);
    void checkAppearences();
    void checkAsteroidBreaksShip();
    void drawEndGame() const;
};

#endif // VIEW_H
