///
//Shmup Game with SDL
//13.11.2013
///

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int FRAMES_PER_SECOND = 30;

const int SHIP_WIDTH = 20;
const int SHIP_HEIGHT = 20;

SDL_Surface *ship = NULL;
SDL_Surface *screen = NULL;

SDL_Event event;

class Ship
{
	private:
	int x, y;

	int xVel, yVel;

	public:
	Ship();

	void handle_input();

	void move();

	void show();
}


