#include"object.h"
#include<math.h>
#include<iostream>
const float t = 0.3;

const float coefficient = 0.5;

SDL_Point desRotateCenter = {10,100};
const float maxSpeedDestroyer = 19.44;

SDL_Point depRotateCenter = {22,4};

int totalTorpedoPos = 2;
int totalWaterBlastRect;
int maxDepthCharge = 26;

using namespace std;

//POSITION

    Position::~Position(){};
    Position Position::operator+(const Position other){
        return {x + other.x,y + other.y};
    }
    Position Position::operator-(const Position other){
        return {x - other.x,y - other.y};
    }
//MOVEMENT

    Movement::~Movement(){}
//COLLIDERECT
CollideRect::CollideRect(){
}


//OBJECT

    Object::Object(){
            pos = new Position;
            mov = new Movement;
            depthChargeHit = 0;
            collideRect = new CollideRect;
            collideRect->collideRect = NULL;

            overTime = 0;

        }
        Object::~Object(){
//            delete pos;
//            delete mov;
//            delete collideRect;
        }
    //OBJECT : POSITION
        void Object::setPos(int xPos,int yPos){
            pos->x = xPos;
            pos->y = yPos;
        }
        void Object::setPos(Position _pos){
            *pos = _pos;
        }
        Position* Object::getPos(){
            return pos;
        }
    //OBJECT : MOVEMENT
        Movement* Object::getMov(){
            return mov;
        }
        void Object::changeMov(float dV,float dAngle){
            mov->maxV += dV;
            if (mov->maxV > maxSpeed) {mov->maxV = maxSpeed;}
            if (mov->maxV < -1*maxSpeed) {mov ->maxV = -1 * maxSpeed;}
            turnAngle += dAngle;
            if (turnAngle > maxTurnAngle) turnAngle= maxTurnAngle;
            if (turnAngle < -1 * maxTurnAngle) turnAngle = -1*maxTurnAngle;
        //    mov->maxV += dV;
        //    if (mov->maxV > maxSpeed) {mov->maxV = maxSpeed;}
        //    turnAngle += dAngle;
        };
        void Object::setmaxSpeed(float _maxSpeed){
            maxSpeed = _maxSpeed;
        }
        float Object::getMaxSpeed(){
            return maxSpeed;
        }
        void Object::Move(){
            angle += (0.1 * turnAngle) * (0.05 * abs(mov->maxV));
            mov->xAcceleration = (mov->maxV * sin(angle*3.14/180) - mov->xSpeed) * coefficient;
            mov->yAcceleration = (-mov->maxV * cos(angle*3.14/180) - mov->ySpeed) * coefficient;

             mov->xSpeed += mov->xAcceleration*t ;

            mov->ySpeed += mov->yAcceleration*t;

            pos->x += mov->xSpeed * t + mov->xAcceleration * t * t /2;
            pos->y += mov->ySpeed * t + mov->yAcceleration * t * t /2;


        }
    //OBJECT : ANGLE
        void Object::setAngle(float _angle){
            angle = _angle;
        }
        float Object::getAngle(){
            return angle;
        }
        void Object::setMaxTurnAngle(float _maxAngle){
            maxTurnAngle = _maxAngle;
        }
    //OBJECT : ALIVE
        void Object::setAlive(bool aliveInp){
            alive = aliveInp;
        };
        bool Object::isAlive(){
            return alive;
        }
    //OBJECT : TEXTURE
        void Object::setTexture(SDL_Texture* Tex){
            tex = Tex;
        };
        void Object::setTextureSize(Size _size){
            textureSize = _size;
        };
        Size Object::getTextureSize(){
            return textureSize;
        }
        void Object::setRotateCenter(SDL_Point* rotatePoint){
            rotateCenter = rotatePoint;
        };
        void Object::render(SDL_Renderer*& renderer,SDL_Rect* clip)
        {
            SDL_Rect renderquad1={pos->x,pos->y,textureSize.w,textureSize.h};
            SDL_RenderCopyEx(renderer,tex,clip,&renderquad1,angle,rotateCenter,SDL_FLIP_NONE);
        }
    //OBJECT : COLLIDERECT
        CollideRect* Object::getCollideRect(){
            for (int i = 0;i < collideRect->number;i++){
                if(collideRect->collideRect != NULL){
                    collideRect->collideRect[i].x = pos->x + getTextureSize().w/2 - collideRect->size->w/2 + collideRect->centerRange[i]*sin(angle*3.14/180);
                    collideRect->collideRect[i].y = pos->y + getTextureSize().h - collideRect->size->h/2 - collideRect->centerRange[i]*cos(angle*3.14/180);
                    collideRect->collideRect[i].w = collideRect->size->w;
                    collideRect->collideRect[i].h = collideRect->size->h;
                }
            }
            return collideRect;
        }
    //
        void Object::hitByDepthCharge(){
            depthChargeHit++;
        }
        int Object::getDepthChargeHit(){
            return depthChargeHit;
        }
        Position Object::getCenterPos(){
            return {pos->x + textureSize.w/2 + textureSize.h/2*sin(3.14*180*getAngle()),pos->y + textureSize.h*(1- 1/2 * cos(getAngle()*3.14/180))};
        }
    //OBJECT : OVER
        void Object::setOverTime(){
            if (overTime != 0) overTime = SDL_GetTicks() + 500;
        }
        bool Object::isOver(){
            if (SDL_GetTicks() > overTime && alive == false){return true;} else {return false;}
        }

//BLAST
    Blast::Blast(){
        texCount = 0;
        setRotateCenter(NULL);
    }
    Blast::~Blast(){}
    Uint8 Blast::getTexCount(){
        return texCount++;
    }
    void Blast::setStartTime(Uint32 _startTime){
        startTime = _startTime;
    }
    Uint32 Blast::getStartTime(){
        return startTime;
    }
    void Blast::setRenderRect(SDL_Rect* _renderRect){
        renderRect = _renderRect;
    }
    SDL_Rect* Blast::getRenderRect(){
        return renderRect;
    }
    void Blast::setTotalRect(int _totalRect){
        totalRect = _totalRect;
        renderRect = new SDL_Rect [totalRect];
    }
    bool Blast::isOver(){
        if (texCount >= totalRect) {return true;} else {return false;}

    }
//TORPEDO
    const float maxSpeedTorpedo = 15.56/rate;

    Torpedo::Torpedo(){
        collidePoint = new Position;
        setRotateCenter(NULL);
        setmaxSpeed(maxSpeedTorpedo);
        startTime = SDL_GetTicks();
    }
    Torpedo::~Torpedo(){}
    bool Torpedo::isOver(){
        if(SDL_GetTicks() - startTime > 20000 || !isAlive()) {return true;} else {return false;}
    }
    Position* Torpedo::getCollidePoint(){
        collidePoint->x = getPos()->x + getTextureSize().w/2 + getTextureSize().h/2*sin(getAngle()*3.14/180);
        collidePoint->y = getPos()->y + getTextureSize().h/2 - getTextureSize().h/2*cos(getAngle()*3.14/180);
        return collidePoint;
    }
//SUBMARINE
    const float maxSubmarineDepth = 200;
    const float minSubmarineDepth = 4.86;
    const float submarineDraft = 4.7;
    const float safeSubmarineDepth = 14.9;
    const float maxTurnAngle = 60;

    const int maxTorpedo = 14;

    SDL_Point subRotateCenter = {7,75};

    Submarine::Submarine(){
        depth = minSubmarineDepth;
        setRotateCenter(&subRotateCenter);
        remainTorpedo = maxTorpedo;

        //set up collideRect
        CollideRect* pCollideRect = getCollideRect();
        pCollideRect->number = 5;
        pCollideRect->collideRect = new SDL_Rect [pCollideRect->number];
        pCollideRect->size = new Size;
        *(pCollideRect->size) = {15,15};
        int centerArr[pCollideRect->number] = {7,24,38,53,68};
        pCollideRect->centerRange = new int [pCollideRect->number];
        for (int i = 0;i < pCollideRect->number;i++){
            pCollideRect->centerRange[i] = centerArr[i];
        }

        setMaxTurnAngle(50);

    }
    Submarine::~Submarine(){

    };

    void Submarine::dive(float dDepth){
        depth += dDepth;
    }
    float Submarine::getDepth(){
        return depth;
    }

    bool Submarine::isHitAble(){
        if (depth < safeSubmarineDepth) {return true;} else {return false;}
    }
    bool Submarine::hadTarget(){
//        if (target == NULL) {return false;} else {return true;}
    }
    void Submarine::findTarget(std::vector<Object*>& targetArr){
        float minDistance = 1000000;
        int targetIndex = 0;
        if (targetArr.size() != 0){
            for (int i = 0;i < targetArr.size();i++){
                float distance = std::sqrt(pow(getPos()->x +getTextureSize().w/2 + getTextureSize().h*sin(getAngle()*3.14/180) - targetArr[i]->getPos()->x,2) +
                                        pow(getPos()->y + getTextureSize().h - getTextureSize().h*cos(getAngle()*3.14/180) - targetArr[i]->getPos()->y,2));
                if (distance < minDistance) {
                    minDistance = distance;
                    targetIndex = i;
                }
            }
        }
        target = targetArr[targetIndex];

    }
    bool Submarine::isTargetDown(){
        if (target->isAlive() == false){return true;} else {return false;}
    }
    Object* Submarine::getTarget(){
        return target;
    }
    void Submarine::approachTarget(SDL_Renderer*& renderer){
        Position targetHeadPos = {target->getPos()->x + target->getTextureSize().w/2 + target->getTextureSize().h*sin(target->getAngle()*3.14/180),
                            target->getPos()->y + target->getTextureSize().h - target->getTextureSize().h*cos(target->getAngle()*3.14/180)};

        float newTurnAngle =  90 + (180/3.14*atan2((targetHeadPos.y - getPos()->y - getTextureSize().h),
                                                  (targetHeadPos.x - getPos()->x - getTextureSize().w/2))) - getAngle();

        changeMov(2,0.005*newTurnAngle);
    }

    void Submarine::fire(std::vector<Torpedo> &torpedoVec,SDL_Texture* torpedoTex,Size torpedoSize){
        Torpedo newTorpedo;
        newTorpedo.setTexture(torpedoTex);
        newTorpedo.setTextureSize(torpedoSize);
        newTorpedo.setPos(getPos()->x + getTextureSize().w/2 + (getTextureSize().h-3)*sin(getAngle()*3.14/180),
                          getPos()->y + getTextureSize().h  - (getTextureSize().h-3)*cos(getAngle()*3.14/180) );
        newTorpedo.getMov()->maxV = maxSpeedTorpedo;
        newTorpedo.setAngle(getAngle());
        remainTorpedo--;
        readyTime = SDL_GetTicks() + 1000;
        torpedoVec.push_back(newTorpedo);

    }
    bool Submarine::isReadyToAttack(){
        Position targetHeadPos = {target->getPos()->x + target->getTextureSize().w/2 + target->getTextureSize().h*sin(target->getAngle()*3.14/180),
                                  target->getPos()->y + target->getTextureSize().h - target->getTextureSize().h*cos(target->getAngle()*3.14/180)};
        float newTurnAngle = getAngle() - 90 - (180/3.14*atan2((targetHeadPos.y - getPos()->y - getTextureSize().h),
                                                    (targetHeadPos.x - getPos()->x - getTextureSize().w/2)));
        float distance = std::sqrt(pow(getPos()->x +getTextureSize().w/2 + getTextureSize().h*sin(getAngle()*3.14/180) - target->getPos()->x,2) +
                                        pow(getPos()->y + getTextureSize().h - getTextureSize().h*cos(getAngle()*3.14/180) - target->getPos()->y,2));
        if (newTurnAngle < 5 && newTurnAngle > -5 && depth < safeSubmarineDepth && SDL_GetTicks() > readyTime && distance < 500 && remainTorpedo > 0)
            {return true;} else {return false;}
    }


//DEPTH CHARGE

    const int maxRangeDepthcharge = 50;

    DepthCharge::DepthCharge(){
        sinkPos.x = 50*cos((getAngle()-90)*3.14/180) + 20*sin((getAngle()-90)*3.14/180);
        sinkPos.y = 50*sin((getAngle()-90)*3.14/180) + 20*(1-sin((getAngle()-90)*3.14/180));
        setRotateCenter(NULL);
        setmaxSpeed(maxSpeedDepthcharge);
        startTime = SDL_GetTicks();
        effectAble = true;
    }
    DepthCharge::~DepthCharge(){

    }
    void DepthCharge::Move(){
        Movement* _mov = getMov();
        Position* _pos = getPos();

        _pos->x += _mov->xSpeed * t + _mov->xAcceleration * t * t /2;
        _pos->y += _mov->ySpeed * t + _mov->yAcceleration * t * t /2;
    }
    void DepthCharge::changeMov(float speed,float dAngle){
        setAngle(dAngle);
        getMov()->xSpeed = speed * sin(dAngle*3.14/180);
        getMov()->ySpeed = -1 * speed* cos(dAngle*3.14/180);

    }
    Uint32 DepthCharge::fliedTime(){
        return SDL_GetTicks() - startTime;
    }
    Position DepthCharge::getSinkPos(){
        return sinkPos;
    }
    bool DepthCharge::isOver(){
        if (effectAble == false) {return true;} else {return false;}
    }
    bool DepthCharge::isEffectAble(){
        return effectAble;
    }
    void DepthCharge::setEffectAble(bool _effectAble){
        effectAble = _effectAble;
    }


//DESTROYER
    const int maxDepthChargeSide = 30;
    const int maxDepthChargeStern = 20;

    int totalDepthChargePos = 3;

    Destroyer::Destroyer(){
        setMaxTurnAngle(60);
        //set up collideRect
        CollideRect *pCoRect = getCollideRect();
        pCoRect->number = 5;
        pCoRect->size = new Size;
        *(pCoRect->size) = {20,20};
        int centerArr[5] = {10,30,50,70,90};
        pCoRect->centerRange = new int [pCoRect->number];
        for (int i = 0;i < pCoRect->number;i++){
            pCoRect->centerRange[i] = centerArr[i];
        }
        pCoRect->collideRect = new SDL_Rect[pCoRect->number];

        readyTime = new Uint32 [totalDepthChargePos];
        for (int i = 0;i < totalDepthChargePos;i++){
            readyTime[i] = 0;
        }
        remainDepthCharge[0] = maxDepthChargeSide;remainDepthCharge[1] = maxDepthChargeStern;

        changeMov(0,0);
        setRotateCenter(&desRotateCenter);
        setmaxSpeed(maxSpeedDestroyer);
    }
    Destroyer::~Destroyer(){

    }

    bool Destroyer::isReady(int launchPos){
        return  SDL_GetTicks() >= readyTime[launchPos];
    }
    void Destroyer::lauchDepthCharge(int launchPos,std::vector<DepthCharge>& depthChargeVec,std::vector<Blast>& blastVec,
            SDL_Texture* depthChargeTex,Size depthChargeSize,SDL_Texture* waterBlastTex,Size waterBlastSize,SDL_Rect* waterBlastRect){
        if (isReady(launchPos)) {
            //LEFT = 0, RIGHT = 1, STERN = 2
            DepthCharge tmp;
            tmp.setTexture(depthChargeTex);
            tmp.setTextureSize(depthChargeSize);

            switch (launchPos){
                case 0:
                    if (remainDepthCharge[0] < 1) break;
                    tmp.setPos(getPos()->x + getTextureSize().w/2 + 20*sin(getAngle()*3.14/180) - tmp.getTextureSize().h/2
                               , getPos()->y + getTextureSize().h - 20*cos(getAngle()*3.14/180));
                    tmp.changeMov(maxSpeedDepthcharge,getAngle()-90);
                    readyTime[launchPos] = SDL_GetTicks()+1000;
                    remainDepthCharge[0]--;
                    break;
                case 1:
                    if (remainDepthCharge[0] < 1) break;
                    tmp.setPos(getPos()->x + getTextureSize().w/2 + 20*sin(getAngle()*3.14/180) - tmp.getTextureSize().h/2,
                                getPos()->y + getTextureSize().h - 20*cos(getAngle()*3.14/180));
                    tmp.changeMov(maxSpeedDepthcharge,getAngle()+90);
                    readyTime[launchPos] = SDL_GetTicks()+1000;
                    remainDepthCharge[0]--;
                    break;
                case 2:
                    if (remainDepthCharge[1] < 1) break;
                    tmp.setPos(getPos()->x + getTextureSize().w/2, getPos()->y + 100);
                    tmp.changeMov(maxSpeedDepthcharge,getAngle()+180);
                    readyTime[launchPos] = SDL_GetTicks()+50;
                    remainDepthCharge[1]--;
                    break;
            }

            depthChargeVec.push_back(tmp);
            Blast scdTmp;
            scdTmp.setTexture(waterBlastTex);
            scdTmp.setTextureSize({80,80});
            scdTmp.setPos(tmp.getPos()->x + maxRangeDepthcharge*sin(tmp.getAngle()*3.14/180) - 40,
                           tmp.getPos()->y - maxRangeDepthcharge*cos(tmp.getAngle()*3.14/180) - 40);
            scdTmp.setTotalRect(12);
            scdTmp.setRenderRect(waterBlastRect);
            scdTmp.setStartTime(SDL_GetTicks()+2000);
            blastVec.push_back(scdTmp);
        }
    }


//CARGOSHIP

    const float maxSpeedCargoShip = 5.9167;

    CargoShip::CargoShip(){
        CollideRect *pCoRect = getCollideRect();
        pCoRect->number = 6;
        pCoRect->size = new Size;
        *(pCoRect->size) = {25,25};
        int centerArr[pCoRect->number] = {12,38,63,88,113,138};
        pCoRect->centerRange = new int [pCoRect->number];
        for (int i = 0;i < pCoRect->number;i++){
            pCoRect->centerRange[i] = centerArr[i];
        }
        pCoRect->collideRect = new SDL_Rect[pCoRect->number];

        changeMov(0,0);
        setmaxSpeed(maxSpeedCargoShip);
    }
    CargoShip::~CargoShip(){};



