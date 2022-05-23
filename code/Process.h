#ifndef PLAY_H_INCLUDED
#define PLAY_H_INCLUDED
#include"object.h"
#include"Texture.h"
#include<vector>
#include<iostream>
#include<math.h>
#include<ctime>
#include <stdlib.h>
using namespace std;

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 700;

bool init();

void loadMenu();

int getMenuRequest(SDL_Event &e);

int query();

void loadText();

void loadMedia();

void initObject();

void handleMouse(SDL_Event& e);

void handleKeyboard();

void objectAct();

void checkCollision();

void screenFix();

void render();

void freeObject();

int getResult();

void close();

void clear();
#endif // PLAY_H_INCLUDED
