#include"Process.h"

int totalFireRect = 12;

SDL_Window* g_window;
SDL_Renderer* renderer;
SDL_Rect backgroundRect = {0,0,1300,700};
TTF_Font* Font = NULL;

SDL_Texture* MenuImage;
SDL_Texture* Background;
SDL_Texture* destroyerTex;
vector<SDL_Texture*> submarineTex;
SDL_Texture* cargoShipTex;
SDL_Texture* torpedoTex;
SDL_Texture* depthChargeTex;
SDL_Texture* waterBlastTex;
SDL_Texture* smt;
SDL_Texture* fireBlastText;
Size backgroundSize(1300,700), destroyerSize(20,100), submarineSize(15,75), cargoSize(25,150),depthChargeSize(9,5),waterBlastSize(80,80),fireBlastSize(80,80),torpedoSize(2,8)
,smtSize;
SDL_Rect* waterBlastRect = new SDL_Rect [12],*fireBlastRect = new SDL_Rect [12];

const float vChangePerPress = 1;
const float degreeChangePerPress = 1;

const float maxSpeedSubmarineUnder = 3.9167/rate;
const int maxSpeedSubmarineOn = 9.11/rate;

const float depthChargeEffectRange = 30/rate;

vector<Destroyer> desVec;
vector<Submarine> subVec;
vector<CargoShip> cargoVec;
vector<DepthCharge> depthChargeVec;
vector<Blast> blastVec;
vector<SDL_Texture*> blipVec;
vector<Torpedo> torpedoVec;

int currentShipIndex = 0;


bool lockScreen = true,movingScreen = false;
Position currentMousePos, oldMousePos;
int currentX,currentY,oldX,oldY;
SDL_Rect playButtonRect = {600,500,100,60};
SDL_Rect quitButtonRect = {600, 550,100,60};
SDL_Rect queryRect = {100,100,1100,500};
SDL_Texture* playButtonOut, *playButtonIn, *quitButtonOut, *quitButtonIn, *destroyerNumberAsk, *submarineNumberAsk, *cargoShipNumberAsk, *okButtonIn, *okButtonOut;

int destroyerNumber,submarineNumber,cargoNumber;

bool init()
{
    srand(time(NULL));

    bool success=true;
    int ret=SDL_Init(SDL_INIT_VIDEO);cout << SDL_GetError();
    if(ret<0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");

    g_window=SDL_CreateWindow("fdssd",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if(g_window==NULL)
    {
        success=false;
    }
    else
    {
        renderer = SDL_CreateRenderer(g_window,-1,SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/ );
        if(renderer == NULL)
        {
            success=false;
        }
        else
        {
            int imgFlags=IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) && imgFlags))
                success=false;
        }
        if(TTF_Init()==-1)
        {
            success=false;
        }
        Font = TTF_OpenFont("Font/ghostclan.ttf",28);
    }
    return success;
}

void loadMenu()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,MenuImage,NULL,NULL);
    SDL_Point* mousePos = new SDL_Point;
    SDL_GetMouseState(&(mousePos->x),&(mousePos->y));

    if (SDL_PointInRect(mousePos,&playButtonRect) == SDL_TRUE)
    {
        SDL_RenderCopy(renderer,playButtonIn,NULL,&playButtonRect);
    }
    else
    {
        SDL_RenderCopy(renderer,playButtonOut,NULL,&playButtonRect);
    }

    if (SDL_PointInRect(mousePos,&quitButtonRect) == SDL_TRUE)
    {
        SDL_RenderCopy(renderer,quitButtonIn,NULL,&quitButtonRect);
    }
    else
    {
        SDL_RenderCopy(renderer,quitButtonOut,NULL,&quitButtonRect);
    }

    SDL_RenderPresent(renderer);


}
int getMenuRequest(SDL_Event &e)
{
    //nothing = -1;quit = 0; play = 1
    SDL_Point* mousePos = new SDL_Point;
    SDL_GetMouseState(&(mousePos->x),&(mousePos->y));

    if (SDL_PointInRect(mousePos,&playButtonRect) == SDL_TRUE)
    {
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {   delete mousePos;
            return 1;
        }
    }

    if (SDL_PointInRect(mousePos,&quitButtonRect) == SDL_TRUE)
    {
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {   delete mousePos;
            return 0;
        }
    }
    delete mousePos;
    return -1;
}

int query(){
    SDL_Rect destroyerNumberAskRect = {200,150,400,60}, submarineNumberAskRect = {200,250,400,60}, cargoShipNumberAskRect = {200,350,400,60},
            destroyerNumberInpRect = {700,150,100,60},submarineNumberInpRect = {700,250,100,60}, cargoShipNumberInpRect = {700,350,100,60},okButtonRect = {1000,450,50,60};
    string input[3];
    int currentStringIndex = -1;
    bool done = false;
    SDL_Point* mousePos = new SDL_Point;
    SDL_Color numberColor = {0,0,0};

    //render query field
    SDL_SetRenderDrawColor(renderer,255,255,255,0);
    SDL_RenderFillRect(renderer,&queryRect);

    SDL_RenderCopy(renderer,destroyerNumberAsk,NULL,&destroyerNumberAskRect);
    SDL_RenderCopy(renderer,submarineNumberAsk,NULL,&submarineNumberAskRect);
    SDL_RenderCopy(renderer,cargoShipNumberAsk,NULL,&cargoShipNumberAskRect);

    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderDrawRect(renderer,&destroyerNumberInpRect);
    SDL_RenderDrawRect(renderer,&submarineNumberInpRect);
    SDL_RenderDrawRect(renderer,&cargoShipNumberInpRect);

    SDL_RenderPresent(renderer);

    SDL_StartTextInput();
    SDL_Event e;

    while (!done){
        SDL_GetMouseState(&(mousePos->x),&(mousePos->y));
        //prees ok button
        if (SDL_PointInRect(mousePos,&okButtonRect) == SDL_TRUE){
            SDL_RenderCopy(renderer,okButtonIn,NULL,&okButtonRect);
        } else {SDL_RenderCopy(renderer,okButtonOut,NULL,&okButtonRect);}

        while (SDL_PollEvent(&e) != 0){
                //press backspace
            if (e.key.keysym.sym == SDLK_BACKSPACE){
                if (currentStringIndex != -1 && input[currentStringIndex].size() > 0){
                    input[currentStringIndex].pop_back();
                }
            }
            //press 0
             else if (e.key.keysym.sym == SDLK_KP_0 || e.key.keysym.sym == SDLK_0 && input[currentStringIndex].size() > 0){
                input[currentStringIndex] += '0';
            }else switch (e.type){
                //press mouse
                case SDL_MOUSEBUTTONDOWN :
                    if (SDL_PointInRect(mousePos,&destroyerNumberInpRect) ==  SDL_TRUE) {currentStringIndex = 0;}
                    else if (SDL_PointInRect(mousePos,&submarineNumberInpRect) ==  SDL_TRUE) {currentStringIndex = 1;}
                    else if (SDL_PointInRect(mousePos,&cargoShipNumberInpRect) ==  SDL_TRUE) {currentStringIndex = 2;}
                    else if (SDL_PointInRect(mousePos,&okButtonRect) == SDL_TRUE) {
                        if (input[0].size() > 0) {destroyerNumber = atoi(input[0].c_str());} else {destroyerNumber = rand() % 5 + 2;}
                        if (input[1].size() > 0) {submarineNumber = atoi(input[1].c_str()); } else {submarineNumber = rand() % 3 + 4;}
                        if (input[2].size() > 0) {cargoNumber = atoi(input[2].c_str());} else {cargoNumber = rand() %10 + 7;}
                        return 1;
                    } else {currentStringIndex = -1;}
                    break;
                //press key
                case SDL_TEXTINPUT:{
                    if (atoi(e.text.text) == 0) {break;}
                    string inp(e.text.text);
                    input[currentStringIndex] += inp;
//                    input[currentStringIndex] += e.text.text;cout << e.text.text;
                    break;}
                //press quit
                case SDL_QUIT:
                    return -1;
                    break;
            };
            //render input field
            SDL_Rect rect[3] = {{701,151,98,58},{701,251,98,58},{701,351,98,58}};
            SDL_SetRenderDrawColor(renderer,255,255,255,0.5);
            for (int i = 0;i < 3;i++){
                SDL_RenderFillRect(renderer,&rect[i]);
                SDL_Rect renderRect = {702,152+100*i,18+22*(input[i].size()-1),60};
                if (input[i].size() > 0) SDL_RenderCopy(renderer,loadFromFont(input[i],renderer,Font,numberColor),NULL,&renderRect);
            }
            SDL_RenderPresent(renderer);
        }
        SDL_RenderPresent(renderer);
    }
//    sdl_ren
    SDL_StopTextInput();
}

void loadText()
{
    SDL_Color inButtonColor = {255, 0, 0};
    SDL_Color outButtonColor = {0, 0, 0};
    playButtonOut = loadFromFont("PLAY",renderer,Font,outButtonColor);
    playButtonIn = loadFromFont("PLAY",renderer,Font,inButtonColor);

    quitButtonIn = loadFromFont("QUIT",renderer,Font,inButtonColor);
    quitButtonOut = loadFromFont("QUIT",renderer,Font,outButtonColor);

    destroyerNumberAsk = loadFromFont("Number of Destroyer:",renderer,Font,outButtonColor);
    submarineNumberAsk = loadFromFont("Number of Submarine:",renderer,Font,outButtonColor);
    cargoShipNumberAsk = loadFromFont("Number of Cargo ship:",renderer,Font,outButtonColor);

    okButtonOut = loadFromFont("OK",renderer,Font,outButtonColor);
    okButtonIn = loadFromFont("OK",renderer,Font,inButtonColor);
}

void loadMedia()
{

    MenuImage = loadImageTexture("img/Menu/53.jpg",renderer);
    Background = loadImageTexture("img/Kablooey!.png",renderer);

    destroyerTex = loadImageTexture("img/Destroyer/warship-side-view-top-view-isolated-white-d-rendering-warship-isolated-white-138462809-removebg-preview.png",renderer);

    submarineTex.push_back(loadImageTexture("img/Submarine/UBoote-TypeVIIC - Copy(1).png",renderer));
    submarineTex.push_back(loadImageTexture("img/Submarine/UBoote-TypeVIIC - Copy(2).png",renderer));
    submarineTex.push_back(loadImageTexture("img/Submarine/UBoote-TypeVIIC - Copy(3).png",renderer));
    submarineTex.push_back(loadImageTexture("img/Submarine/UBoote-TypeVIIC - Copy(4).png",renderer));
    submarineTex.push_back(loadImageTexture("img/Submarine/UBoote-TypeVIIC - Copy(5).png",renderer));
    submarineTex.push_back(loadImageTexture("img/Submarine/UBoote-TypeVIIC - Copy(6).png",renderer));


    cargoShipTex = loadImageTexture("img/Carrier/Blueprints_Ships_Ships__Other__SS_John_W_Brown__Liberty_Ship_-removebg-preview.png",renderer);

    depthChargeTex = loadImageTexture("img/New folder/barrel-oil-pixel-art-cask-8-bit-pixelate-vector-illustration-2BM30JH.png",renderer);

    waterBlastTex = loadImageTexture("img/blast/26490249(1).png",renderer);

    fireBlastText = loadImageTexture("img/blast/74d36f24070811.5632fd987dd8f.png",renderer);

    torpedoTex = loadImageTexture("img/Torpedo/large_000000_-_Copy-removebg-preview.png",renderer);


}

void initObject()
{
    //generate ships number


    for (int i = 0; i < 12; i++)
    {
        waterBlastRect[i].x = 166*(i%6);
        waterBlastRect[i].y = 166*(i/6);
        waterBlastRect[i].w = 166;
        waterBlastRect[i].h = 166;
    }
    for (int i = 0; i < 6; i++)
    {
        fireBlastRect[i].x = 150*(i%4);
        fireBlastRect[i].y = 150*(i/4);
        fireBlastRect[i].w = 150;
        fireBlastRect[i].h = 150;
    }


    for (int i = 0; i < destroyerNumber; i++)
    {
        Destroyer  newDes;
        newDes.setPos(920+630*(i%2),5100 - 125*(i/2));
        newDes.setTexture(destroyerTex);
        newDes.setTextureSize(destroyerSize);
        desVec.push_back(newDes);
    }

    for (int i = 0; i < cargoNumber; i++)
    {
        CargoShip newCargoShip;
        newCargoShip.setTexture(cargoShipTex);
        newCargoShip.setTextureSize(cargoSize);
        newCargoShip.setPos(1000 + 100*(i%5),5100 - 175*(i/5));
        newCargoShip.changeMov(newCargoShip.getMaxSpeed(),0);
        cargoVec.push_back(newCargoShip);
    }
    vector<Object*> targetArr;
    for (int i = 0; i < desVec.size(); i++)
    {
        targetArr.push_back(&desVec[i]);
    }
    for (int i = 0; i < cargoVec.size(); i++)
    {
        targetArr.push_back(&cargoVec[i]);
    }

    Position startPosSubmarine = {rand() % 1000,rand() % 1000 + 3000};
    float startAngleSubmarine = rand() % 360;
    for (int i = 0; i < submarineNumber; i++)
    {
        Submarine newSub;
        newSub.setAngle(startAngleSubmarine);
        newSub.setPos(startPosSubmarine.x + 80*i,startPosSubmarine.y);
        newSub.findTarget(targetArr);
        newSub.setTextureSize(submarineSize);
        newSub.setmaxSpeed(maxSpeedSubmarineOn);
        subVec.push_back(newSub);
    }

    smt = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,2408,5496);

}



void handleMouse(SDL_Event& e)
{
    if (e.type == SDL_MOUSEWHEEL)
    {
        short coefficient = 5;
        if ((backgroundRect.w - 2*coefficient < 2000 && e.wheel.y < 0) || (backgroundRect.w - 2*coefficient > 800) && e.wheel.y > 0)
        {
            backgroundRect.x += coefficient*e.wheel.y;
            backgroundRect.y += coefficient*e.wheel.y;
            backgroundRect.h -= 2*coefficient*e.wheel.y;
            backgroundRect.w -= 2*coefficient*e.wheel.y;

        }
    } else
    if((e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) && !lockScreen)
    {
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            movingScreen = true;
            SDL_GetMouseState(&oldX,&oldY);
        }
        if (e.type == SDL_MOUSEMOTION && movingScreen)
        {
            SDL_GetMouseState(&currentX,&currentY);
            backgroundRect.x -= currentX - oldX;
            backgroundRect.y -= currentY - oldY;
            oldX = currentX;
            oldY = currentY;
        }
        if (e.type == SDL_MOUSEBUTTONUP)
        {
            movingScreen = false;
        }
    }
}

void handleKeyboard()
{
//    cout << "start handle keyboard" << endl;
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    if( currentKeyStates[ SDL_SCANCODE_UP ] )
    {
        desVec[currentShipIndex].changeMov(vChangePerPress,0);
    }
    if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
    {
        desVec[currentShipIndex].changeMov(-vChangePerPress,0);
    }
    if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
    {
        desVec[currentShipIndex].changeMov(0,-degreeChangePerPress);
    }
    if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
    {
        desVec[currentShipIndex].changeMov(0,degreeChangePerPress);
    }
    if( currentKeyStates[ SDL_SCANCODE_TAB ] )
    {
        currentShipIndex++;
        currentShipIndex %= desVec.size();
        while(!desVec[currentShipIndex].isAlive())
        {
            currentShipIndex++;
            currentShipIndex %= desVec.size();
        }
    }
    //LEFT = 0, RIGHT = 1, STERN = 2
    if( currentKeyStates[ SDL_SCANCODE_Z ] )
    {
        desVec[currentShipIndex].lauchDepthCharge(0, depthChargeVec, blastVec,
                depthChargeTex,depthChargeSize,waterBlastTex,waterBlastSize,waterBlastRect);
        subVec[0].fire(torpedoVec,torpedoTex,torpedoSize);
    }
    if( currentKeyStates[ SDL_SCANCODE_X ] )
    {
        desVec[currentShipIndex].lauchDepthCharge(1, depthChargeVec, blastVec,
                depthChargeTex,depthChargeSize,waterBlastTex,waterBlastSize,waterBlastRect);
    }
    if( currentKeyStates[ SDL_SCANCODE_C ] )
    {
        desVec[currentShipIndex].lauchDepthCharge(2, depthChargeVec, blastVec,
                depthChargeTex,depthChargeSize,waterBlastTex,waterBlastSize,waterBlastRect);
    }
    if( currentKeyStates[ SDL_SCANCODE_T ] )
    {
        lockScreen = !lockScreen;
    }
}

void objectAct()
{
    for (int i = 0; i < subVec.size(); i++)
    {
        subVec[i].approachTarget(renderer);
        subVec[i].Move();
        if (subVec[i].isReadyToAttack())
        {
            subVec[i].fire(torpedoVec,torpedoTex,torpedoSize);
        }
    }
    for (int i = 0;i < torpedoVec.size();i++){
       torpedoVec[i].Move();
    }
    for (int i = 0; i < cargoVec.size(); i++)
    {
        cargoVec[i].Move();
    }
    for (int i = 0; i < depthChargeVec.size(); i++)
    {
        depthChargeVec[i].Move();
    }
    for (int i = 0; i < desVec.size(); i++)
    {
        desVec[i].Move();
    }
}

void checkCollision()
{
    std::vector<CollideRect*> collideRectArr;
    std::vector<Object*> objectArr;
    for (int i = 0; i < desVec.size(); i++)
    {
        collideRectArr.push_back(desVec[i].getCollideRect());
        objectArr.push_back(&desVec[i]);
    }
    for (int i = 0; i < cargoVec.size(); i++)
    {
        collideRectArr.push_back(cargoVec[i].getCollideRect());
        objectArr.push_back(&cargoVec[i]);
    }
    for (int i = 0; i < collideRectArr.size(); i++)
    {
        for (SDL_Rect* j = collideRectArr[i]->collideRect; j < collideRectArr[i]->collideRect+(collideRectArr[i]->number); j++)
        {
            SDL_RenderDrawRect(renderer,j);
        }
    }
    for (int i = 0; i < subVec.size(); i++)
    {
        for (SDL_Rect* j = subVec[i].getCollideRect()->collideRect; j != (subVec[i].getCollideRect()->collideRect)+5; j++)
        {
            SDL_RenderDrawRect(renderer,j);
        }
    }
//    //destroyer vs cargo
    for (int i = 0; i < collideRectArr.size()-1; i++)
    {
        for (int j = i+1; j < collideRectArr.size(); j++)
        {
            for (int k = 0; k < collideRectArr[i]->number; k++)
            {
                for (int l = 0; l < collideRectArr[j]->number; l++)
                {
                    if (SDL_HasIntersection(&(collideRectArr[i]->collideRect[k]),&(collideRectArr[j]->collideRect[l])) == SDL_TRUE)
                    {
                        objectArr[i]->setAlive(false);objectArr[i]->setOverTime();
                        objectArr[j]->setAlive(false);objectArr[j]->setOverTime();
                        Blast fireBlastTmp;
                        fireBlastTmp.setTotalRect(totalFireRect);
                        fireBlastTmp.setRenderRect(fireBlastRect);
                        fireBlastTmp.setTexture(fireBlastText);
                        fireBlastTmp.setTextureSize(fireBlastSize);
                        fireBlastTmp.setPos((collideRectArr[i]->collideRect[k].x + collideRectArr[j]->collideRect[l].x)/2,
                                            (collideRectArr[i]->collideRect[k].y + collideRectArr[j]->collideRect[l].y)/2);
                        fireBlastTmp.setStartTime(SDL_GetTicks());
                        blastVec.push_back(fireBlastTmp);
                        j++;
                        k = collideRectArr[i]->number;
                        break;
                    }
                }
            }
        }
    }
    //submarine vs (cargo,destroyer)
    for (int i = 0; i < subVec.size(); i++)
    {
        if (subVec[i].isHitAble())
        {
            CollideRect* p = subVec[i].getCollideRect();
            for (int j = 0; j < collideRectArr.size(); j++)
            {
                for (SDL_Rect* subRect = p->collideRect; subRect != p->collideRect + p->number; subRect++)
                {
                    for (int l = 0; l < collideRectArr[j]->number; l++)
                    {
                        if (SDL_HasIntersection(subRect,&(collideRectArr[j]->collideRect[l])) == SDL_TRUE)
                        {
                            subVec[i].setAlive(false);subVec[i].setOverTime();
                            objectArr[j]->setAlive(false);objectArr[j]->setOverTime();
                            Blast fireBlastTmp;
                            fireBlastTmp.setTotalRect(totalFireRect);
                            fireBlastTmp.setRenderRect(fireBlastRect);
                            fireBlastTmp.setTexture(fireBlastText);
                            fireBlastTmp.setTextureSize({80,80});
                            fireBlastTmp.setPos((subRect->x + collideRectArr[j]->collideRect[l].x)/2,
                                                (subRect->y + collideRectArr[j]->collideRect[l].y)/2);
                            fireBlastTmp.setStartTime(SDL_GetTicks());
                            blastVec.push_back(fireBlastTmp);
                            j++;
                            subRect = p->collideRect + p->number-1;
                            break;
                        }
                    }
                }
            }
        }
    }
    //torpedo vs (cargo,destroyer)
    for (int i = 0; i < torpedoVec.size(); i++)
    {
        SDL_Point* torpedoHead = new SDL_Point;
        torpedoHead->x = torpedoVec[i].getCollidePoint()->x;
        torpedoHead->y = torpedoVec[i].getCollidePoint()->y;

        for (int j = 0; j < collideRectArr.size(); j++)
        {
            for (int k = 0; k < collideRectArr[j]->number; k++)
            {
                if (SDL_PointInRect(torpedoHead,&(collideRectArr[j]->collideRect[k])) == SDL_TRUE)
                {
                    objectArr[j]->setAlive(false);objectArr[j]->setOverTime();
                    torpedoVec[i].setAlive(false);torpedoVec[i].setOverTime();
                    Blast fireBlastTmp;
                    fireBlastTmp.setTotalRect(totalFireRect);
                    fireBlastTmp.setRenderRect(fireBlastRect);
                    fireBlastTmp.setTexture(fireBlastText);
                    fireBlastTmp.setTextureSize(fireBlastSize);
                    fireBlastTmp.setPos(torpedoHead->x - fireBlastSize.w/2,torpedoHead->y - fireBlastSize.h/2);
                    fireBlastTmp.setStartTime(SDL_GetTicks());
                    blastVec.push_back(fireBlastTmp);
                    j = collideRectArr.size();
                    break;
                }
            }
        }
        delete torpedoHead;
    }
    //Depthcharge vs ship,submarine

    for (vector<DepthCharge>::iterator depthCharge = depthChargeVec.begin(); depthCharge != depthChargeVec.end(); depthCharge++)
    {
        if (depthCharge->isEffectAble() && depthCharge->fliedTime() > 1000)
        {
//        Effect on ship
            for (int j = 0; j < collideRectArr.size(); j++)
            {
                bool hitted = false;
                for (int k = 0; k < collideRectArr[j]->number; k++)
                {
                    SDL_Rect* rect = collideRectArr[j]->collideRect + k;
                    float distance = sqrt( pow (depthCharge->getCenterPos().x - rect->x - rect->w,2) +
                                           pow (depthCharge->getCenterPos().y - rect->y - rect->h,2));
                    if (distance <= depthChargeEffectRange)
                    {
                        objectArr[j]->hitByDepthCharge();
                        hitted = true;

                        if (objectArr[j]->getDepthChargeHit() > 1)
                        {

                            objectArr[j]->setAlive(false);objectArr[j]->setOverTime();
                            Blast fireBlastTmp;
                            fireBlastTmp.setTotalRect(totalFireRect);
                            fireBlastTmp.setRenderRect(fireBlastRect);
                            fireBlastTmp.setTexture(fireBlastText);
                            fireBlastTmp.setTextureSize(fireBlastSize);
                            fireBlastTmp.setPos(objectArr[j]->getCenterPos().x - fireBlastSize.w/2,objectArr[j]->getCenterPos().y - fireBlastSize.h/2);
                            fireBlastTmp.setStartTime(SDL_GetTicks());
                            blastVec.push_back(fireBlastTmp);
                        }
                    }
                    if (hitted == true) break; //complete current object
                }
                if (hitted == true) continue; //next object
            }
            //Effect on submarine
            for (int j = 0; j < subVec.size(); j++)
            {
                bool hitted = false;
                CollideRect* p = subVec[j].getCollideRect();
                for (int k = 0; k < p->number; k++)
                {
                    SDL_Rect* rect = p->collideRect + k;
                    float distance = sqrt( pow (depthCharge->getCenterPos().x - rect->x - rect->w,2) +
                                           pow (depthCharge->getCenterPos().y - rect->y - rect->h,2)) - sqrt(2)*rect->w/2;
                    if (distance <= depthChargeEffectRange)
                    {
                        subVec[j].hitByDepthCharge();
                        hitted = true;
//                        cout << "get hit";
                        if (subVec[j].getDepthChargeHit() > 1)
                        {

                            subVec[j].setAlive(false);subVec[j].setOverTime();
                            Blast fireBlastTmp;
                            fireBlastTmp.setTotalRect(totalFireRect);
                            fireBlastTmp.setRenderRect(fireBlastRect);
                            fireBlastTmp.setTexture(fireBlastText);
                            fireBlastTmp.setTextureSize(fireBlastSize);
                            fireBlastTmp.setPos(subVec[j].getCenterPos().x - fireBlastSize.w/2,subVec[j].getCenterPos().y - fireBlastSize.h/2);
                            fireBlastTmp.setStartTime(SDL_GetTicks());
                            blastVec.push_back(fireBlastTmp);
                        }
                    }
                    if (hitted == true) break; //complete current object
                }
                if (hitted == true) continue; //next object
            }
            depthCharge->setEffectAble(false);
        }
    }
    for (int i = 0; i < subVec.size(); i++)
    {
        if (subVec[i].isTargetDown())
        {
            subVec[i].findTarget(objectArr);
        }
    }
}

void screenFix()
{
    if (lockScreen)
    {
        backgroundRect.x = desVec[currentShipIndex].getPos()->x + desVec[currentShipIndex].getTextureSize().w/2 + desVec[currentShipIndex].getTextureSize().h*sin(desVec[currentShipIndex].getAngle()*3.14/180) - SCREEN_WIDTH/2;
        backgroundRect.y = desVec[currentShipIndex].getPos()->y + desVec[currentShipIndex].getTextureSize().h - desVec[currentShipIndex].getTextureSize().h*cos(desVec[currentShipIndex].getAngle()*3.14/180) - SCREEN_HEIGHT/2;
    }
    if (backgroundRect.x < 0) backgroundRect.x = 0;
    if (backgroundRect.y < 0) backgroundRect.y = 0;
    if (backgroundRect.x + backgroundRect.w > 2408) {backgroundRect.x = 2408 - backgroundRect.w;}
    if (backgroundRect.y + backgroundRect.h > 5496) {backgroundRect.y = 5496 - backgroundRect.h;}
}
void render()
{
    SDL_RenderCopy(renderer,Background,NULL,NULL);

    for (int i = 0; i < desVec.size(); i++)
    {

        desVec[i].render(renderer,NULL);
    }

    for (int i = 0; i < depthChargeVec.size(); i++)
    {
        if (depthChargeVec[i].fliedTime() < 1000)
        {
            depthChargeVec[i].render(renderer,NULL);
        }
    }

    for (int i = 0; i < blastVec.size(); i++)
    {
        if (SDL_GetTicks() >= blastVec[i].getStartTime())
        {
            SDL_Rect* pRect = blastVec[i].getRenderRect();
            blastVec[i].render(renderer,pRect + blastVec[i].getTexCount());
        }
    }
    for (int i = 0; i < torpedoVec.size(); i++)
    {
        torpedoVec[i].render(renderer,NULL);
    }
    for (int i = 0; i < subVec.size(); i++)
    {
        subVec[i].setTexture(submarineTex[0]);
        subVec[i].render(renderer,NULL);
    }
    for (int i = 0; i < cargoVec.size(); i++)
    {
        cargoVec[i].render(renderer,NULL);
    }


    SDL_SetRenderTarget(renderer,NULL);
    SDL_RenderCopy(renderer,smt,&backgroundRect,NULL);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer,smt);
}

void freeObject()
{
    //free over object
    for (int i = 0; i < depthChargeVec.size(); i++)
    {
        if (depthChargeVec[i].isOver())
        {
            depthChargeVec.erase(depthChargeVec.begin());
        }
    }
    {
        int i = 0;
        while (i < blastVec.size())
        {
            if (blastVec[i].isOver())
            {
                blastVec.erase(blastVec.begin()+i);
            }
            else
            {
                i++;
            }
        }
    }
    {
        int i = 0;
        while (i < desVec.size())
        {
            if (desVec[i].isOver())
            {
                desVec.erase(desVec.begin()+i);
            }
            else
            {
                i++;
            }
        }
    }

    {
        int i = 0;
        while (i < cargoVec.size())
        {
            if (cargoVec[i].isOver())
            {
                cargoVec.erase(cargoVec.begin()+i);
            }
            else
            {
                i++;
            }
        }
    }
    {
        int i = 0;
        while (i < subVec.size())
        {
            if (subVec[i].isOver())
            {
                subVec.erase(subVec.begin()+i);
            }
            else
            {
                i++;
            }
        }
    }
    {
        int i = 0;
        while (i < torpedoVec.size())
        {
            if (torpedoVec[i].isOver())
            {
                torpedoVec.erase(torpedoVec.begin()+i);
            }
            else
            {
                i++;
            }
        }
    }
}
int getResult()
{
    //win = 1,lose = -1
    if (subVec.size() == 0)
    {
        return 1;
    }
    if (desVec.size() == 0)
    {
        return -1;
    }
    return 0;
}

void close()
{
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}
void clear()
{
    subVec.clear();
    desVec.clear();
    torpedoVec.clear();
    blastVec.clear();
    subVec.clear();
    cargoVec.clear();
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer,NULL);
}
