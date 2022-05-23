#include"Texture.h"
#include<iostream>

const int COLOR_KEY_R = 180, COLOR_KEY_G = 160, COLOR_KEY_B = 150;

Size::Size(int _w,int _h)
{
    w = _w;
    h = _h;
}
void Size::print()
{
    std::cout << w << ' ' << h;
}
Size Size::operator/(Uint8 rate)
{
    w /= rate;
    h /= rate;
}


SDL_Texture* loadImageTexture(std::string path,SDL_Renderer*& renderer)
{
    SDL_Texture* new_texture=NULL;
    SDL_Surface* load_surface=IMG_Load(path.c_str());
    if(load_surface !=NULL)
    {
        SDL_SetColorKey(load_surface,SDL_TRUE,SDL_MapRGB(load_surface->format,COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
        new_texture=SDL_CreateTextureFromSurface(renderer, load_surface);
        SDL_FreeSurface(load_surface);
    }
    else
    {
        printf("load media failed! %s",SDL_GetError());
    }
    return new_texture;
}
SDL_Texture* loadFromFont(std::string textureText,SDL_Renderer*& renderer,TTF_Font* Font, SDL_Color textColor)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid( Font, textureText.c_str(), textColor );
    SDL_Texture* fontTexture = NULL;
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        fontTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( fontTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    //Return success
    return fontTexture;
}


