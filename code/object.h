#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED
#include"Texture.h"
#include<vector>

const int rate = 1;
const float maxSpeedDepthcharge = 13;

enum {left = 0, right = 1, stern = 2 } firePos;

struct Position
{
    float x,y;
    ~Position();
    Position operator+(const Position other);
    Position operator-(const Position other);
};

////SDL_Point desRotateCenter = {10,98};

struct Movement {
    float maxV = 0;

    float xSpeed = 0;
    float ySpeed = 0;

    float xAcceleration = 0;
    float yAcceleration = 0;

    ~Movement();

};
struct CollideRect{
    Uint8 number;
    Size* size;
    int* centerRange;
    Size* collideRectSize;
    SDL_Rect* collideRect;

    CollideRect();
};


class Object{
    Position* pos;
    Movement* mov;
    bool alive = true;
    float turnAngle;
    float angle = 0;
    float maxTurnAngle;

    float maxSpeed;

    SDL_Point* rotateCenter;
    SDL_Texture* tex;
    Size textureSize;

    CollideRect* collideRect;

    int depthChargeHit;

    Uint32 overTime;
public:
    Object();
    ~Object();

    void setPos(int xPos,int yPos);
    void setPos(Position _pos);
    Position* getPos();

    Movement* getMov();
    void changeMov(float dV,float dAngle);
    void setmaxSpeed(float _maxSpeed);
    float getMaxSpeed();
    void Move();

    void setAngle(float _angle);
    float getAngle();
    void setMaxTurnAngle(float _maxAngle);

    void setAlive(bool Alive);
    bool isAlive();

    void setTexture(SDL_Texture* Tex);
    void setTextureSize(Size _size);
    Size getTextureSize();
    void setRotateCenter(SDL_Point* rotatePoint);
    void render(SDL_Renderer*& renderer,SDL_Rect* clip);

    CollideRect* getCollideRect();

    void hitByDepthCharge();
    int getDepthChargeHit();

    Position getCenterPos();

    void setOverTime();
    bool isOver();
};

class controlableObject : public Object
{
public:

};

class Blast : public Object{
    int texCount;
    int totalRect;
    Position startPos;
    Uint32 startTime;
    SDL_Rect* renderRect;
public:
    Blast();
    ~Blast();
    Uint8 getTexCount();

    void setStartTime(Uint32 _startTime);
    Uint32 getStartTime();
    SDL_Rect* getRenderRect();
    void setRenderRect(SDL_Rect* _renderRect);
    void setTotalRect(int _totalRect);

    bool isOver();
};

class Torpedo :public Object
{
    Position* collidePoint;
    Uint32 startTime;
public :
    Torpedo();
    ~Torpedo();
    bool isOver();

    Position* getCollidePoint();
};

class Submarine : public controlableObject
{
    float depth;
    Object* target;
    int remainTorpedo;
    int readyTime;
public :
    Submarine();
    ~Submarine();
    void dive(float dDepth);
    float getDepth();

    bool hadTarget();
    bool isHitAble();
    void findTarget(std::vector<Object*>& targetArr);
    bool isTargetDown();
    Object* getTarget();
    void approachTarget(SDL_Renderer*& renderer);

    void fire(std::vector<Torpedo> &torpedoVec,SDL_Texture* torpedoTex,Size torpedoSize);
    bool isReadyToAttack();
};


class DepthCharge : public controlableObject{
    Uint32 startTime;
    Position sinkPos;
    bool effectAble;
public :
    DepthCharge();
    ~DepthCharge();

    void Move();
    void changeMov(float speed,float dAngle);
    Uint32 fliedTime();
    Position getSinkPos();
    bool isEffectAble();
    void setEffectAble(bool _effectAble);
    bool isOver();
};

class CargoShip : public controlableObject
{
public :
    CargoShip();
    ~CargoShip();
};


class Destroyer : public controlableObject
{
    Uint32* readyTime;
    Position rotateCenter = {5,49};
    int remainDepthCharge[2];
public:
    Destroyer();
    ~Destroyer();

    bool isReady(int launchPos);
    void lauchDepthCharge(int launchPos,std::vector<DepthCharge>& depthChargeVec,std::vector<Blast>& blastVec,
                          SDL_Texture* depthChargeTex,Size depthChargeSize,SDL_Texture* waterBlastTex,Size waterBlastSize,SDL_Rect* renderRect);

};

#endif // OBJECT_H_INCLUDED
