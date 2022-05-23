#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include<SDL.h>
#include<string>
#include<SDL_image.h>
#include<SDL_ttf.h>


struct Size{
    int w;
    int h;

    Size(int _w = 0,int _h = 0);
    void print();
    Size operator/(Uint8 rate);
};

SDL_Texture* loadImageTexture(std::string path,SDL_Renderer*& renderer);
SDL_Texture* loadFromFont(std::string textureText,SDL_Renderer*& renderer,TTF_Font* Font, SDL_Color textColor);

#endif // TEXTURE_H_INCLUDED
