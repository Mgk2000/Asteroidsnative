#ifndef VIEW_H
#define VIEW_H
#include <globject.h>
//#include "game.h"
#include <list>
#include <vector>
#include "points.h"
#include "random.h"
#include "mat4.h"
#include "text.h"
#include "texture.h"
#include "bonus.h"

class Ship;
class Gun;
class Bullet;
class Random;
class Asteroid;
class GeometryEngine;
class Patrol;
class Background;
class Bonus;
class Mutex;
class Sand;
class Explosion;
class Target;
class ARectangle;
class BitmapText;
class RoundedRectangle;
class PauseContinue;
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
    enum Sounds
    {
        NONE,
        SHOOT,
        BREAK,
        PATROL,
        BONUS,
        EXPLOSION,
        SHIPBREAK,
        ENDGAME,
		LEVELDONE
    };

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
    int drawFrame();
    bool initializeGL(bool _russian);
    void resizeGL(int w, int h);
    void paintGL();
    int width, height;
    std::list<TouchEvent>touches;
    void onTouchEvent(int what, int x, int y);
    Ship* getShip() {return ship;}
    void setPPause(bool p);
    bool getPause() {return _pause;}
    void newGame();
    int lives() const {return _lives;}
    int scores() const {return _scores;}
    inline bool gameIsOver() const {return _lives <= 0;}
    void addTexture (int w, int h, const char* data,
                     int kind, bool transparentWhite);
    const std::vector <Texture *> &textures() {return _textures;}
    void freeBonus(Asteroid*);
    long long currTime() const {return _currTime;}
    const std::vector<Bonus*> &shipBonuses() const {return _shipBonuses;}
    const std::list<Asteroid*> &getAsteroids() const {return asteroids;}
    std::list<Bullet*> &getBullets()  {return bullets;}
    Bonus* shipBonus() const {return _shipBonus;}
    //Text* getText() const {return text;}
    BitmapText* getBitmapText() const {return bitmapText;}
    void checkBonusExpired();
    void clearGame();
    Patrol* getPatrol() const {return patrol;}
    void addBullet(Bullet* bullet);
    void createTargets();
private:
    bool _pause;

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
	void deleteBullet(Bullet* bullet);
	void createSplinters(Asteroid* asteroid);
    std::list <Bonus*> bonuses;
    std::list <Sand*> sands;
    Patrol* patrol;
    Ship* ship;
    Background * background;
    Mutex* mutex;
    void drawCurrentResult() const;
	GLuint _program;
	Gun * gun;
	float aspect;
	Random _random1, _random2;
	int nticks;
	int dieticks;
    int levelDoneTicks;
	int _matrixlocation, _vertexlocation, _colorlocation;
    long long _currTime, _lastTime;
    long long _startTime;
    int period;
    //Text *text;
    BitmapText* bitmapText;
    int _lives;
    int _scores;
    void sound(Sounds is) {if (is>maxSound) maxSound=is;}
    Sounds maxSound;
    void checkEndGame();
    void moveObjects(float delta);
    void checkAppearences();
    void checkAsteroidBreaksShip();
    void checkCatchBonus();
    void catchBonus(Bonus* bonus);
    void drawEndGame() const;
    void drawLevelDone() const;
    std::vector <Texture *> _textures;
    TextureProgram* _textureProgram;
    std::vector<Bonus*> _shipBonuses;
    std::list <Explosion*> explosions;
    Bonus* _shipBonus;
    long long _shipBonusExpiredTime;
    void setShipBonus(Bonus* bonus, int msec);
    void drawShipBonuses();
    void clearShipBonuses();
    Bonus::Kind shipBonusKind() const;
    void deleteShipBonus(Bonus* bonus);
    void drawBonusBar();
    Bonus* touchedShipBonus(float x, float y);
    bool shipUnbreakable() const;
    void destroyAsteroid(Asteroid* asteroid, bool total);
    float _smallExplosionRadius;
    void smallExplosion();
    void bigExplosion();
    std::vector<Target*> targets;
    bool _gameRunning;
    //-----------Level
    int _level;
    int _maxTargets;
    int _maxCountedLevel;
    float _levelAppearencePeriod;
    int _levelTargets;
    int _levelPatrolBreaks , _patrolBreaks ;
    struct LevelBonus
    {
        int num;
        int catched;
        char name[16];
        Bonus* bonus;
    };
    std::vector<LevelBonus*> _levelBonuses;
    int _levelScores;
    int _levelStartScores;
    int _levelTargetBreaks, _targetBreaks;
    float _levelBonusProbability;
    bool _levelDone;
    Random * levelRandom;
    void initLevels();
    void calcLevel(int __level);
    float calcAppearenceFrequency(int __level);
    void startLevel(int l);
    bool checkLevelDone();
    void drawLevelTodo();
    void drawLevelCompleting() const;
    bool _showingDialog;
    void showLevelDialog();
    float _okButtonLeft, _okButtonTop, _okButtonRight, _okButtonBottom;
    void processDialogTouch(float fx, float fy);
    //-----------------
    ARectangle* _rectangle;
    RoundedRectangle* _roundedRect, * _wideRoundedRect;
    long long okPressTime;
    PauseContinue* pauseContinue;
    bool russian;
};

#endif // VIEW_H
