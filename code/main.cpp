#include"Process.h"
#include"Texture.h"
#include<iostream>
#include<vector>
#include<math.h>
#include<cstdlib>

using namespace std;
//const int MaxRadarRange = 128000;
// rate 1m : 1pixel = 1:1







int main( int argc, char* args[] )
{

	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
	    loadMedia();
        loadText();

        bool quit = false;

        SDL_Event e;


        while( !quit )
        {
            loadMenu();

            while(SDL_PollEvent(&e) != 0){

                int menuRequest = getMenuRequest(e);
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }

                //quit = 0; play = 1
                if (menuRequest == 1){
                    if (query() == -1) {quit = true;}
                    initObject();
                    int result = 0;
                    while(result == 0 && !quit){
                        while (SDL_PollEvent( &e ) != 0)
                        {
                            if( e.type == SDL_QUIT )
                            {
                                quit = true;
                            } else {handleMouse(e);}

                        }
                        handleKeyboard();
                        objectAct();
                        checkCollision();
                        screenFix();
                        render();
                        freeObject();

                        result = getResult();
                        SDL_Delay(100);
                    }
                    clear();
                } else if (menuRequest == 0) {quit = true;}
            }
		}
	}
	close();

	return 0;
}
